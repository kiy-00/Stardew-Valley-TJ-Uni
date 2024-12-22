#include "Animal.h"
#include "cocos2d.h"
#include <cstdlib>

USING_NS_CC;

// ��ʼ����̬��Ա
std::unordered_set<std::string> Animal::_loadedPlists;

/**
 * @brief ��ȡ���ж�������
 */
const std::unordered_map<std::string, AnimalConfig>& Animal::getAnimalConfigs()
{
    static std::unordered_map<std::string, AnimalConfig> configs = {
        {"chicken", {"chicken", "egg.png", "chicken_idle_", "chicken_walk_", 45, 15}},
        {"sheep", {"sheep", "wool.png", "sheep_idle_", "sheep_walk_", 60, 15}},
        // ��������Ӹ��ද�����͵�����
    };
    return configs;
}

/**
 * @brief ����ָ�����͵�Animal����
 */
Animal* Animal::createWithType(const std::string& typeName)
{
    auto it = getAnimalConfigs().find(typeName);
    if (it == getAnimalConfigs().end()) {
        CCLOG("Unknown animal type: %s", typeName.c_str());
        return nullptr;
    }

    Animal* animal = new (std::nothrow) Animal();
    if (animal && animal->initWithConfig(it->second))
    {
        animal->autorelease();
        return animal;
    }
    CC_SAFE_DELETE(animal);
    return nullptr;
}

/**
 * @brief ���ݶ������ó�ʼ������
 */
bool Animal::initWithConfig(const AnimalConfig& config)
{
    if (!Sprite::init())
        return false;

    _config = config;
    _favor = 0;
    _canProduce = false;
    _currentAction = nullptr;

    // ���ó�ʼ��������Ϊ
    playIdleAnimation();
    scheduleUpdate();        // ע����·���
    startRandomMove();       // ��ʼ����ƶ�

    return true;
}

/**
 * @brief ��Ҵ�������ʱ���ã����Ӻøж�
 */
void Animal::touchAnimal()
{
    _favor += _config.favorIncrement; // �����������Ӻøж�
    if (_favor >= _config.produceFavorThreshold) {
        _canProduce = true;  // �ﵽ��ֵ����������
    }
}

/**
 * @brief ÿ֡�����߼�������Ƿ��������
 */
void Animal::update(float dt)
{
    if (_canProduce) {
        tryProduce();
    }
}

/**
 * @brief ����������Ʒ
 */
void Animal::tryProduce() {
	if (_favor >= _config.produceFavorThreshold) {
		if (produceCallback) {
			produceCallback(_config.produceItem, getPosition());
		}
		_favor = 0;          // ���úøж�
		_canProduce = false; // �ر�����״̬
		CCLOG("%s produced!", _config.produceItem.c_str());
	}
}


/**
 * @brief ���ſ���״̬����
 */
void Animal::playIdleAnimation()
{
    loadAnimationFrames("idle", Direction::DOWN); // Ĭ�ϳ�����
    std::string prefix = _config.idleAnimationPrefix + "down_";

    auto animate = createAnimate(prefix, 2, 0.5f); // ���ɶ���
    this->stopAllActions();
    this->runAction(RepeatForever::create(animate)); // ѭ�����Ŷ���
}

/**
 * @brief �����ƶ�״̬����
 * @param dir �ƶ�����
 */
void Animal::playWalkAnimation(Direction dir)
{
    loadAnimationFrames("walk", dir);
    std::string prefix = _config.walkAnimationPrefix;

    switch (dir) {  // ���ݷ���ѡ��֡ǰ׺
    case Direction::UP: prefix += "up_"; break;
    case Direction::DOWN: prefix += "down_"; break;
    case Direction::LEFT: prefix += "left_"; break;
    case Direction::RIGHT: prefix += "right_"; break;
    }

    auto animate = createAnimate(prefix, 4, 0.2f); // ���ɶ���
    this->stopAllActions();
    this->runAction(RepeatForever::create(animate)); // ѭ�����Ŷ���
}

/**
 * @brief ��ʼ����ƶ��߼�
 */
void Animal::startRandomMove()
{
    float delayTime = RandomHelper::random_real(3.0f, 6.0f); // �����ʱ
    this->runAction(Sequence::create(
        DelayTime::create(delayTime),
        CallFunc::create([this]() {
            this->doRandomMove();
            }),
        nullptr));
}

/**
 * @brief ִ��һ������ƶ�
 */
void Animal::doRandomMove()
{
	if (!this->getParent()) return;
	auto parentSize = this->getParent()->getContentSize(); // ��ȡ���ڵ��С
	Vec2 startPos = getPosition(); // ��ǰ����λ��

	// �����������Ŀ��λ�õķ�Χ�����ٵ�ԭ����1/10��
	Vec2 targetPos(
		RandomHelper::random_real(startPos.x - 10.0f, startPos.x + 10.0f),
		RandomHelper::random_real(startPos.y - 10.0f, startPos.y + 10.0f)
	);

	// ȷ��Ŀ��λ���ڸ��ڵ㷶Χ��
	targetPos.x = clampf(targetPos.x, 50.0f, parentSize.width - 50.0f);
	targetPos.y = clampf(targetPos.y, 50.0f, parentSize.height - 50.0f);

	Direction moveDir = getDirectionForMovement(startPos, targetPos); // �����ƶ�����
	playWalkAnimation(moveDir);

	// �����ƶ�����ʱ�䣨���ٵ�ԭ����1/10��
	float moveDuration = 0.2f;

	auto moveTo = MoveTo::create(moveDuration, targetPos);
	auto seq = Sequence::create(moveTo, CallFunc::create([this]() {
		this->onMoveFinished();
		}), nullptr);
	this->runAction(seq); // ִ�ж�������
}

/**
 * @brief ����ƶ�������Ļص�
 */
void Animal::onMoveFinished()
{
    playIdleAnimation();
    startRandomMove(); // ������һ������ƶ�
}

/**
 * @brief ����֡ǰ׺���ɶ���
 * @param framePrefix ֡ǰ׺
 * @param frameCount ֡��
 * @param delayPerUnit ÿ֡�ӳ�ʱ��
 * @return ���ɵ�Animate����
 */
Animate* Animal::createAnimate(const std::string& framePrefix, int frameCount, float delayPerUnit)
{
    Vector<SpriteFrame*> frames;
    for (int i = 1; i <= frameCount; i++) {
        char frameName[60];
        sprintf(frameName, "%s%02d.png", framePrefix.c_str(), i);
        if (!framePrefix.empty()) frameName[0] = toupper(frameName[0]); // ֡��������ĸת��д
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (frame) {
            frames.pushBack(frame);
        }
        else {
            CCLOG("Frame not found: %s", frameName);
        }
    }

    auto animation = Animation::createWithSpriteFrames(frames, delayPerUnit);
    return Animate::create(animation);
}

/**
 * @brief ����ָ��״̬�ͷ���Ķ���֡
 * @param state ����״̬�����л��ƶ���
 * @param direction ���ﳯ��
 */
void Animal::loadAnimationFrames(const std::string& state, Direction direction)
{
    std::string directionStr;
    switch (direction) {
    case Direction::UP: directionStr = "up"; break;
    case Direction::DOWN: directionStr = "down"; break;
    case Direction::LEFT: directionStr = "left"; break;
    case Direction::RIGHT: directionStr = "right"; break;
    }

    std::string animalTypeStr = _config.typeName;
    std::string stateStr = state;
    std::string plistPath = "animals/" + animalTypeStr + "_" + stateStr + "_" + directionStr + ".plist";

    // ֻ����δ���ع���plist
    if (_loadedPlists.find(plistPath) == _loadedPlists.end()) {
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plistPath);
        _loadedPlists.insert(plistPath);
    }
}

/**
 * @brief ���������յ�����ƶ�����
 * @param start �������
 * @param end �յ�����
 * @return �ƶ�����
 */
Animal::Direction Animal::getDirectionForMovement(const Vec2& start, const Vec2& end)
{
    float dx = end.x - start.x;
    float dy = end.y - start.y;
    if (fabs(dx) > fabs(dy)) {
        return (dx > 0) ? Direction::RIGHT : Direction::LEFT;
    }
    else {
        return (dy > 0) ? Direction::UP : Direction::DOWN;
    }
}
