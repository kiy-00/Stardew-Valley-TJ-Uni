// GameScene.cpp
#include "GameScene.h"

USING_NS_CC;

Scene* GameScene::createScene() {
	return GameScene::create();
}
GameScene::~GameScene() {
	if (weatherSystem) {
		weatherSystem->release();  // �ͷ�����ϵͳ����
		weatherSystem = nullptr;
	}
	if (seasonSystem) {
		weatherSystem->release();  // �ͷż���ϵͳ����
		weatherSystem = nullptr;
	}
}
bool GameScene::init() {
	if (!Scene::init()) {
		return false;
	}

	// ��ȡ����ϵͳʵ��
	weatherSystem = WeatherSystem::getInstance();

	// ��ȡ����ϵͳʵ��
	seasonSystem = SeasonSystem::getInstance();

	// ��ʼ��ũ������
	initFarmlands();

	// ��ʼ�����߰�ť
	initToolButtons();

	//��ʼ��ũ���ﰴť
	initCropButtons();
	

	// ע�ᴥ���¼�
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//���ö�ʱ����
	this->scheduleUpdate();

	return true;
}
bool GameScene::initTileMap() {
	// ������Ƭ��ͼ
	tmxMap = TMXTiledMap::create("island_.tmx");
	if (tmxMap) {
		this->addChild(tmxMap);
		tmxMap->setName("tmxMap");
		tmxMap->setPosition(cocos2d::Vec2(0, 0));

		CCLOG("��ͼ��С: %.0f x %.0f", tmxMap->getMapSize().width,
			tmxMap->getMapSize().height);
		CCLOG("ͼ���С: %.0f x %.0f", tmxMap->getTileSize().width,
			tmxMap->getTileSize().height);

		return true;
	}
	else {
		CCLOG("���ص�ͼʧ�ܣ���ǰ����Ŀ¼: %s",
			FileUtils::getInstance()->getDefaultResourceRootPath().c_str());
		return false;
	}
}
void GameScene::initFarmlands() {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// ����3x3��ũ������
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			Position pos(i, j);
			Farmland* farmland = Farmland::create(pos);

			weatherSystem->addObserver(farmland);//ע������ϵͳ�۲���
			seasonSystem->addObserver(farmland);//ע�Ἶ��ϵͳ�۲���

			// ����ũ��λ�ã���������ռ�Ź��߰�ť
			farmland->setPosition(origin.x + 200 + i * 64, origin.y + 100 + j * 64);
			this->addChild(farmland);
			m_farmlands.push_back(farmland);//�洢��GameScene��ũ��������
		}
	}
}

void GameScene::initToolButtons() {
	// �������߰�ť
	auto createToolButton = [this](const std::string& text, const std::string& toolType, float posY) {
		auto label = Label::createWithTTF(text, "fonts/arial.ttf", 20);
		auto button = MenuItemLabel::create(
			label,
			[this, toolType](Ref* sender) { onToolButtonClicked(sender, toolType); }
		);
		button->setPosition(Vec2(100, posY));
		return button;
	};

	auto menu = Menu::create(
		createToolButton("Hoe", "hoe", 300),
		createToolButton("Water", "wateringcan", 250),
		createToolButton("Fertilizer", "fertilizer", 200),
		nullptr
	);

	// ����ջ�ť
	auto harvestButton = MenuItemLabel::create(
		Label::createWithTTF("Harvest", "fonts/arial.ttf", 20),
		[this](Ref* sender) {
			if (m_currentTool) {
				delete m_currentTool;
			}
			m_currentTool = nullptr;  // ����״̬�����ջ�
			m_isHarvestMode = true;   // �����ջ�ģʽ
		}
	);

	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);
	harvestButton->setPosition(Vec2(100, 150));
	menu->addChild(harvestButton);

}
void GameScene::initCropButtons() {
	// ������ֲ��ť
	auto createCropButton = [this](const std::string& text, CropType cropType, float posY) {
		auto label = Label::createWithTTF(text, "fonts/arial.ttf", 20);
		auto button = MenuItemLabel::create(
			label,
			[this, cropType](Ref* sender) { onCropButtonClicked(sender, cropType); }
		);
		button->setPosition(Vec2(100, posY));  // ���ڹ��߰�ť����
		return button;
		};

	auto menu = Menu::create(
		createCropButton("Plant Corn", CropType::CORN, 100),
		//createCropButton("Plant Potato", CropType::POTATO, 100),
		//createCropButton("Plant Tomato", CropType::TOMATO, 50),
		nullptr
	);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);
}

// �޸Ĵ�����������֧����ֲ
bool GameScene::onTouchBegan(Touch* touch, Event* event) {
	auto farmland = getFarmlandAtPosition(touch->getLocation());
	if (!farmland) return false;

	if (m_isHarvestMode) {
		// �ջ�ģʽ
		if (farmland->harvest()) {
			// �ջ�ɹ�
			m_isHarvestMode = false;  // �˳��ջ�ģʽ
		}
	}
	else if (m_currentTool) {
		// ʹ�ù���
		farmland->handleTool(m_currentTool);
	}
	else {
		// ��ֲģʽ
		if (farmland->isTilled() && farmland->isWatered() && !farmland->hasCrop()) {
			farmland->plant(m_currentCropType);
		}
	}

	return true;
}

void GameScene::onToolButtonClicked(Ref* sender, const std::string& toolType) {
	if (m_currentTool) {
		delete m_currentTool;
	}
	m_currentCropType = CropType::NONE;
	m_currentTool = ToolManager::getInstance()->createTool(toolType);
	CCLOG("Selected tool: %s", toolType.c_str());
}

void GameScene::onCropButtonClicked(Ref* sender, CropType cropType) {
	m_currentCropType = cropType;
	CCLOG("Selected crop: %d", static_cast<int>(cropType));

	// ��������ﱻѡ�У��Զ��л�������״̬
	if (m_currentTool) {
		delete m_currentTool;
		m_currentTool = nullptr;
	}
}

Farmland* GameScene::getFarmlandAtPosition(const Vec2& position) {
	for (auto farmland : m_farmlands) {
		if (farmland->getBoundingBox().containsPoint(position)) {
			return farmland;
		}
	}
	return nullptr;
}

void GameScene::update(float dt) {
	// ����ʱ�����ţ���ѡ��
	float timeScale = 150.0f;  // 1��ʵ�� = 1��Ϸ����
	float gameTime = 0.01f * timeScale;

	// ��������ũ�������
	for (auto farmland : m_farmlands) {
		farmland->update(gameTime);
		weatherSystem->update(gameTime);
		seasonSystem->update(gameTime);
	}
}