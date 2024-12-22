#include "cocostudio/CocoStudio.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "platform/CCFileUtils.h"
#include "FishingLayer.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

////////////////////////////////////////////////////////////////
// FishingLayerProperty�ṹ�������ڴ洢��������Ĳ�����״̬��Ϣ��
// ����ʱ���ʱ�����뻬����˶���Χ���ٶȡ��������ƶ��ٶȵ����ԡ�
////////////////////////////////////////////////////////////////
FishingLayerProperty::FishingLayerProperty()
{
	this->timeCounter = 0.0f;                // ʱ���ʱ,��λ��
	this->fishYMin = 0;                      // ����͸߶�
	this->fishYMax = 0;                      // ����߸߶�
	this->fishRandomY = 0;                   // ������߶�(�������ڻ�����Χ�ڵ����λ��)
	this->fishMoveSpeed = 200;               // ���ƶ��ٶ�(����/��)
	this->sliderYMin = 0;                    // ������͸߶�
	this->sliderYMax = 0;                    // ������߸߶�
	this->sliderOriginMoveSpeed = 200;       // ����ԭʼ�ƶ��ٶ�(����/��)
	this->sliderMoveSpeed = 0;               // ���鵱ǰ�ƶ��ٶ�(����/��)
	this->loadingBarOriginMoveSpeed = 0.3f;  // ������ԭʼ�����ٶ�(1~100)
	this->loadingBarMoveSpeed = 0;           // ��������ǰ�����ٶ�(1~100)
}

////////////////////////////////////////////////////////////
// ����FishingLayer�����ķ�����
// @return Scene* ����һ������FishingLayer�ĳ�������
////////////////////////////////////////////////////////////
Scene* FishingLayer::createScene()
{
	auto scene = Scene::create();
	auto layer = FishingLayer::create();
	scene->addChild(layer);
	return scene;
}

////////////////////////////////////////////////////////////
// ��ʼ��FishingLayer�����UI��ʼ�����¼����������ݳ�ʼ���붨ʱ���ȡ�
////////////////////////////////////////////////////////////
bool FishingLayer::init()
{
	baseNode = nullptr;
	this->initUI();
	this->initKeyListener();
	this->initSchedule();
	this->initData();
	return true;
}

////////////////////////////////////////////////////////////
// ��ʼ��UI����������csb�ļ������ý�������ʼֵ�ȡ�
////////////////////////////////////////////////////////////
void FishingLayer::initUI()
{
	// ��CSB�ļ��д����ڵ���
	Node* pRoot = CSLoader::getInstance()->createNodeWithFlatBuffersFile("FishingLayer.csb");
	auto bg = pRoot->getChildByName("bg");
	addChild(pRoot);
	this->baseNode = pRoot;

	// ��ʼ��LoadingBar�ĳ�ʼ����
	Node* LoadingBar = baseNode->getChildByName("LoadingBar");
	this->LoadingBarUI = (ui::LoadingBar*)LoadingBar;
	this->LoadingBarUI->setPercent(0);
}

////////////////////////////////////////////////////////////
// ��ʼ��������������������¼��봥���¼���
// �����¼������¿ո��û��������ƶ����ɿ��ո��û��������ƶ���
// �����¼���������ʼ�����ƶ����������������ƶ���
////////////////////////////////////////////////////////////
void FishingLayer::initKeyListener()
{
	// ���������¼�������
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(FishingLayer::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(FishingLayer::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithFixedPriority(keyboardListener, -11);
	_keyboardListener = keyboardListener;

	// �������㴥���¼�������
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(FishingLayer::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(FishingLayer::onTouchEnded, this);
	_touchListener = listener;
	_eventDispatcher->addEventListenerWithFixedPriority(listener, 1);
}

////////////////////////////////////////////////////////////
// ��ʼ���������ݣ������������뻬���������λ�á�������Ŀ��λ�õȡ�
// �˺�����init()�е��á�
////////////////////////////////////////////////////////////
void FishingLayer::initData()
{
	if (baseNode == nullptr) {
		return;
	}

	Node* bg = baseNode->getChildByName("bg");
	Node* shutiao = baseNode->getChildByName("shutiao");
	Node* yu = baseNode->getChildByName("yu");
	Node* huakuai = baseNode->getChildByName("huakuai");
	Node* LoadingBar = baseNode->getChildByName("LoadingBar");

	// �����������ƶ�������λ��
	int fishYMin = shutiao->getPosition().y
		- shutiao->getAnchorPoint().y * shutiao->getContentSize().height
		+ yu->getContentSize().height * yu->getAnchorPoint().y;

	int fishYMax = shutiao->getPosition().y
		+ (1 - shutiao->getAnchorPoint().y) * shutiao->getContentSize().height
		+ yu->getContentSize().height * (yu->getAnchorPoint().y - 1);

	_fishingLayerProperty.fishYMin = fishYMin;
	_fishingLayerProperty.fishYMax = fishYMax;

	// �趨��ĳ�ʼλ�ú����Ŀ��λ��
	int currentPositionY = yu->getPositionY();
	int fishMoveSpeed = _fishingLayerProperty.fishMoveSpeed;
	double randomNumber = static_cast<double>(rand()) / RAND_MAX;
	int fishRandomY = (int)(randomNumber * (fishYMax - fishYMin)) + fishYMin;
	int sign = ((fishRandomY - currentPositionY) >= 0) ? 1 : -1;
	fishMoveSpeed = sign * abs(fishMoveSpeed);

	_fishingLayerProperty.fishRandomY = fishRandomY;
	_fishingLayerProperty.fishMoveSpeed = fishMoveSpeed;
	yu->setPositionY(fishYMin); // ��ʼ�����λ��������

	// ���㻬�������ƶ�������λ��
	int sliderYMin = shutiao->getPosition().y
		- shutiao->getAnchorPoint().y * shutiao->getContentSize().height
		+ huakuai->getContentSize().height * huakuai->getAnchorPoint().y;

	int sliderYMax = shutiao->getPosition().y
		+ (1 - shutiao->getAnchorPoint().y) * shutiao->getContentSize().height
		+ huakuai->getContentSize().height * (huakuai->getAnchorPoint().y - 1);

	_fishingLayerProperty.sliderYMin = sliderYMin;
	_fishingLayerProperty.sliderYMax = sliderYMax;
}

////////////////////////////////////////////////////////////
// ��ʼ����ʱ��������scheduleUpdate()����ÿ֡����update������
////////////////////////////////////////////////////////////
void FishingLayer::initSchedule()
{
	scheduleUpdate();
}

////////////////////////////////////////////////////////////
// ��ȡ��������ĺ�������ǰʵ��Ϊ�գ��ɸ�����Ҫ���ز�����
// @return ����һ��std::unordered_map<std::string, int>�Ĳ�����
////////////////////////////////////////////////////////////
std::unordered_map<std::string, int> FishingLayer::getParams()
{
	std::unordered_map<std::string, int> hashMap;
	return hashMap;
}

////////////////////////////////////////////////////////////
// ���浱ǰ�����Ϣ
// @param ���������ز���
////////////////////////////////////////////////////////////
void FishingLayer::setTargetFish(const FishInfo& fish)
{
	_targetFish = fish; // ����Ŀ������Ϣ
}

////////////////////////////////////////////////////////////
// onExit�ص������˳���ʱ������Դ��������ȡ�
////////////////////////////////////////////////////////////
void FishingLayer::onExit()
{
	Layer::onExit();

	// ȷ���Ƴ����̼�����
	if (_keyboardListener) {
		_eventDispatcher->removeEventListener(_keyboardListener);
		_keyboardListener = nullptr;
	}

	// ȷ���Ƴ�����������
	if (_touchListener) {
		_eventDispatcher->removeEventListener(_touchListener);
		_touchListener = nullptr;
	}

	// ֹͣ��ʱ������
	this->unscheduleUpdate();
	this->baseNode = nullptr;
}


////////////////////////////////////////////////////////////
// ÿ֡���õĸ��º���������dt(֡ʱ��)�����㡢���顢��������Ԫ�ص�״̬��
// @param dt ÿ֡����ʱ�䣬��λ��
////////////////////////////////////////////////////////////
void FishingLayer::update(float dt)
{
	this->updateFish(dt);
	this->updateSlider(dt);
	this->updateLoadingBar(dt);
}

////////////////////////////////////////////////////////////
// �������״̬�������������֮������ζ���
// @param dt ÿ֡����ʱ�䣬��λ��
////////////////////////////////////////////////////////////
void FishingLayer::updateFish(float dt)
{
	Node* bg = baseNode->getChildByName("bg");
	Node* shutiao = baseNode->getChildByName("shutiao");
	Node* yu = baseNode->getChildByName("yu");
	Node* huakuai = baseNode->getChildByName("huakuai");
	Node* LoadingBar = baseNode->getChildByName("LoadingBar");

	float timeCounter = _fishingLayerProperty.timeCounter;
	int fishYMin = _fishingLayerProperty.fishYMin;
	int fishYMax = _fishingLayerProperty.fishYMax;
	int fishRandomY = _fishingLayerProperty.fishRandomY;
	int fishMoveSpeed = _fishingLayerProperty.fishMoveSpeed;

	int currentPositionY = yu->getPositionY();

	// ������Ѿ��ﵽ���Ŀ��λ��(�򼴽�����)���������������Ŀ��λ��
	if ((currentPositionY >= fishRandomY + dt * fishMoveSpeed && fishMoveSpeed >= 0)
		|| (currentPositionY <= fishRandomY + dt * fishMoveSpeed && fishMoveSpeed < 0))
	{
		currentPositionY = yu->getPositionY();
		fishMoveSpeed = _fishingLayerProperty.fishMoveSpeed;
		double randomNumber = static_cast<double>(rand()) / RAND_MAX;
		fishRandomY = (int)(randomNumber * (fishYMax - fishYMin)) + fishYMin;
		int sign = ((fishRandomY - currentPositionY) >= 0) ? 1 : -1;
		fishMoveSpeed = sign * abs(fishMoveSpeed);
		_fishingLayerProperty.fishRandomY = fishRandomY;
		_fishingLayerProperty.fishMoveSpeed = fishMoveSpeed;
	}

	// �����ٶȸ������Y����
	currentPositionY += dt * fishMoveSpeed;
	yu->setPositionY(currentPositionY);
}

////////////////////////////////////////////////////////////
// ���»����λ�ã�����sliderMoveSpeed�ƶ����ұ������޶���Χ�ڡ�
// @param dt ÿ֡����ʱ�䣬��λ��
////////////////////////////////////////////////////////////
void FishingLayer::updateSlider(float dt)
{
	Node* huakuai = baseNode->getChildByName("huakuai");
	int currentPositionY = huakuai->getPositionY();

	int sliderYMin = _fishingLayerProperty.sliderYMin;
	int sliderYMax = _fishingLayerProperty.sliderYMax;
	int sliderMoveSpeed = _fishingLayerProperty.sliderMoveSpeed;

	// ���㻬�����λ�ò�������������֮��
	currentPositionY = currentPositionY + sliderMoveSpeed * dt;
	currentPositionY = MIN(currentPositionY, sliderYMax);
	currentPositionY = MAX(currentPositionY, sliderYMin);
	huakuai->setPositionY(currentPositionY);
}

////////////////////////////////////////////////////////////
// ���½�������״̬�������������غ�ʱ��������������������١�
// ������ȴﵽ100%���ʾʤ����
// @param dt ÿ֡����ʱ�䣬��λ��
////////////////////////////////////////////////////////////
void FishingLayer::updateLoadingBar(float dt)
{
	Node* LoadingBar = baseNode->getChildByName("LoadingBar");
	ui::LoadingBar* LoadingBarUI = static_cast<ui::LoadingBar*>(LoadingBar);

	float percent = LoadingBarUI->getPercent();

	// ��ȡ��ͻ���İ�Χ�У������ж������Ƿ��ཻ
	Node* yu = baseNode->getChildByName("yu");
	Rect yuBoundingBox = yu->getBoundingBox();
	yuBoundingBox.origin = yu->convertToWorldSpaceAR(Vec2());

	Node* huakuai = baseNode->getChildByName("huakuai");
	Rect huakuaiBoundingBox = huakuai->getBoundingBox();
	huakuaiBoundingBox.origin = huakuai->convertToWorldSpaceAR(Vec2());

	// ������������ཻ�����������ӣ��������
	if (huakuaiBoundingBox.intersectsRect(yuBoundingBox)) {
		_fishingLayerProperty.loadingBarMoveSpeed = _fishingLayerProperty.loadingBarOriginMoveSpeed;
	}
	else {
		_fishingLayerProperty.loadingBarMoveSpeed = -_fishingLayerProperty.loadingBarOriginMoveSpeed;
	}

	// ���½������İٷֱȣ��������� 0 �� 100 ֮��
	percent = percent + _fishingLayerProperty.loadingBarMoveSpeed;
	percent = MAX(percent, 0);
	percent = MIN(percent, 100);
	LoadingBarUI->setPercent(percent);

	// ƽ��������ɫ��С�� 30% ʱΪ��ɫ�����ڵ��� 30% ʱ�𽥹��ɵ���ɫ
	Color3B color = interpolateColor(percent);
	LoadingBarUI->setColor(color);

	printf("%.3f\n", percent);

	// ��Ϸ�����߼�
	if (percent >= 100.0f) {
		// �ɹ�����������Ϸ
		this->unscheduleUpdate();
		this->stopAllActions();

		if (onGameEnd) onGameEnd(true, _targetFish); // �ɹ�
		
		// �ӳ����٣�ȷ���ص���ɺ�
		this->runAction(Sequence::create(
			DelayTime::create(0.1f), 
			CallFunc::create([this]() { this->removeFromParent(); }),
			nullptr
		));
		return;
	}
	else if (percent <= 0.0f) {
		// ʧ�ܽ���������Ϸ
		this->unscheduleUpdate();
		this->stopAllActions();

		if (onGameEnd) onGameEnd(false, _targetFish); // ʧ��
		
		// �ӳ����٣�ȷ���ص���ɺ�
		this->runAction(Sequence::create(
			DelayTime::create(0.1f),
			CallFunc::create([this]() { this->removeFromParent(); }),
			nullptr
		));
		return;
	}
}

////////////////////////////////////////////////////////////
// ���ݽ��Ȱٷֱ�ƽ����ֵ��ɫ��
// @param percent ��ǰ���Ȱٷֱȣ�0~100��
// @return Color3B ƽ����ֵ�����ɫ
////////////////////////////////////////////////////////////
Color3B FishingLayer::interpolateColor(float percent)
{
	// ������ʼ��ɫ����ɫ����Ŀ����ɫ����ɫ��
	Color3B startColor = Color3B::RED;
	Color3B endColor = Color3B::GREEN;

	// �����ֵ������ȷ���� 0 �� 1 ֮��
	float t = percent / 100.0f;
	t = MAX(0.0f, MIN(t, 1.0f));

	// ���Բ�ֵ���� R��G��B ����
	GLubyte r = startColor.r + t * (endColor.r - startColor.r);
	GLubyte g = startColor.g + t * (endColor.g - startColor.g);
	GLubyte b = startColor.b + t * (endColor.b - startColor.b);

	return Color3B(r, g, b);
}

////////////////////////////////////////////////////////////
// ����������������ݴ����direction(1��-1)���趨������ƶ��ٶȡ�
// ������ƶ��ٶȱ�����Ϊ���ƶ��ٶȵ������������ݷ�������������á�
// @param direction �ƶ�����(1��ʾ����-1��ʾ����)
////////////////////////////////////////////////////////////
void FishingLayer::handleSliderAction(int direction)
{
	float timeCounter = _fishingLayerProperty.timeCounter;
	int fishYMin = _fishingLayerProperty.fishYMin;
	int fishYMax = _fishingLayerProperty.fishYMax;
	int fishRandomY = _fishingLayerProperty.fishRandomY;
	int fishMoveSpeed = _fishingLayerProperty.fishMoveSpeed;
	int sliderYMin = _fishingLayerProperty.sliderYMin;
	int sliderYMax = _fishingLayerProperty.sliderYMax;
	int sliderOriginMoveSpeed = _fishingLayerProperty.sliderOriginMoveSpeed;
	int sliderMoveSpeed = _fishingLayerProperty.sliderMoveSpeed;

	// ���û����ƶ��ٶ�Ϊ���ƶ��ٶȵ�������������directionȷ������
	_fishingLayerProperty.sliderMoveSpeed = direction * 2 * abs(fishMoveSpeed);
}

////////////////////////////////////////////////////////////
// ���̰����¼��ص����������ո������ʱ��ʹ�������������ƶ���
// @param code ���̰�����
// @param event �¼�����
////////////////////////////////////////////////////////////
void FishingLayer::onKeyPressed(EventKeyboard::KeyCode code, Event* event)
{
	log("onKeyPressed, keycode: %d", code);
	if (code == EventKeyboard::KeyCode::KEY_SPACE) {
		handleSliderAction(1);  // �����ƶ�
	}
}

////////////////////////////////////////////////////////////
// �����ɿ��¼��ص����������ո���ɿ�ʱ��ʹ�����򷴷����ƶ���
// @param code ���̰�����
// @param event �¼�����
////////////////////////////////////////////////////////////
void FishingLayer::onKeyReleased(EventKeyboard::KeyCode code, Event* event)
{
	log("onKeyReleased, keycode: %d", code);
	if (code == EventKeyboard::KeyCode::KEY_SPACE) {
		handleSliderAction(-1);  // �����ƶ�
	}
}

////////////////////////////////////////////////////////////
// ������ʼ�¼��ص����������û�������Ļʱ��ʹ�������������ƶ���
// @param touch ��������
// @param event �¼�����
// @return true ��ʾ�¼�������
////////////////////////////////////////////////////////////
bool FishingLayer::onTouchBegan(Touch* touch, Event* event)
{
	handleSliderAction(1);  // �����ƶ�
	return true;
}

////////////////////////////////////////////////////////////
// ���������¼��ص����������û�����������Ļʱ��ʹ�����򷴷����ƶ���
// @param touch ��������
// @param event �¼�����
////////////////////////////////////////////////////////////
void FishingLayer::onTouchEnded(Touch* touch, Event* event)
{
	handleSliderAction(-1);  // �����ƶ�
}
