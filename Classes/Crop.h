#include "TimeSeasonSystem.h"
#include "WeatherSystem.h"
#include "FarmlandTile.h"

//// ����ö�٣�Ӧ���� TimeSeasonSystem.h �ж��壩
//enum class Season {
//    SPRING,
//    SUMMER,
//    FALL,
//    WINTER
//};
//
//// ���ڱ仯�¼��ṹ��Ӧ���� TimeSeasonSystem.h �ж��壩
//struct SeasonChangeEvent {
//    Season previousSeason;
//    Season newSeason;
//    int year;
//    int day;
//};

class Crop : public Node {
protected:
    enum class GrowthStage {
        SEED,
        STAGE1,
        STAGE2,
        STAGE3,
        DEAD
    };

    float growthProgress = 0.0f;  // ��������(0-100)
    GrowthStage currentStage = GrowthStage::SEED;
    bool isWatered = false;
    bool isFertilized = false;
    int deathCounter = 0;  // ����׷�ٲ�����������ʱ��

    FarmlandTile* farmland = nullptr;
    std::string cropType;  // "berry", "carrot", "pepper", "wheat"

    // ����Ӱ��ϵ��
    float seasonGrowthModifier = 1.0f;
    float weatherGrowthModifier = 1.0f;

    // ��������(�������д)
    struct CropProperties {
        float growthRate = 1.0f;        // ���������ٶ�
        float waterNeed = 50.0f;        // ��ˮ��
        float fertilityNeed = 30.0f;    // �����
        std::vector<float> stageThresholds = { 25.0f, 50.0f, 75.0f, 100.0f };  // �����׶���ֵ

        // ��ͬ����������ϵ��
        std::map<std::string, float> weatherModifiers = {
            {"sunny", 1.2f},
            {"cloudy", 1.0f},
            {"rainy", 1.1f},
            {"snowy", 0.5f},
            {"foggy", 0.8f}
        };

        // ��ͬ���ڵ�����ϵ��
        std::map<std::string, float> seasonModifiers = {
            {"spring", 1.2f},
            {"summer", 1.0f},
            {"fall", 0.8f},
            {"winter", 0.5f}
        };
    } properties;

public:
    static Crop* create(const std::string& type, FarmlandTile* tile);
    virtual bool init(const std::string& type, FarmlandTile* tile);

    void update(float dt) override;

    bool canHarvest() const;
    virtual void harvest();

protected:
    void updateSprite();
    virtual float calculateGrowthRate();
    void checkGrowthStage();
    bool checkEnvironmentConditions();

    void setupEventListeners();
    void removeEventListeners();

    virtual ~Crop() {
        removeEventListeners();
    }
};

