#include "FishDataFactory.h"


////////////////////////////////////////////////////////////
// �������������ڽ����κ����������Ϊ���������������¼��
// @param fishTable ������Ϣ��
// @param season ���ڡ�
// @param name ������ơ�
// @param sprite ��ľ���ͼ�����ơ�
// @param gold ��Ľ�Ҽ�ֵ��
// @param hp �������ֵ��
// @param energy �������ֵ��
////////////////////////////////////////////////////////////
static void addFishForBothWeathers(std::vector<FishInfo>& fishTable, const std::string& season, const std::string& name,
	const std::string& sprite, int gold, int hp, int energy) {
	fishTable.push_back({ season,"����",name,sprite,gold,hp,energy });  // ��������¼
	fishTable.push_back({ season,"����",name,sprite,gold,hp,energy });  // ��������¼
}

////////////////////////////////////////////////////////////
// Ϊÿ����������м��ڡ����������ļ�¼
// @param fishTable ������Ϣ��
// @param name ������ơ�
// @param sprite ��ľ���ͼ�����ơ�
// @param gold ��Ľ�Ҽ�ֵ��
// @param hp �������ֵ��
// @param energy �������ֵ��
////////////////////////////////////////////////////////////
static void addFishAllSeasonsAndWeathers(std::vector<FishInfo>& fishTable, const std::string& name,
	const std::string& sprite, int gold, int hp, int energy) {
	// ���м���
	std::vector<std::string> seasons = { "����","�ļ�","�＾","����" };
	// ��������
	std::vector<std::string> weathers = { "����","����" };

	// �������м��ں�������������Ӧ�����¼
	for (auto& s : seasons) {
		for (auto& w : weathers) {
			fishTable.push_back({ s,w,name,sprite,gold,hp,energy });
		}
	}
}

////////////////////////////////////////////////////////////
// ����������������������
// FishInfo : ���ڡ���������������ҡ�����ֵ������ֵ
////////////////////////////////////////////////////////////
std::vector<FishInfo> FishDataFactory::createFishData() {
	std::vector<FishInfo> fishTable;

	/* -------------- ���� ------------- */
	// �κ�����
	addFishForBothWeathers(fishTable, "����", "Anchovy", "fish/Anchovy.png", 50, 20, 30);
	addFishForBothWeathers(fishTable, "����", "Herring", "fish/Herring.png", 80, 20, 30);

	// ����
	fishTable.push_back({ "����","����","Catfish","fish/Catfish.png",300,20,30 });
	fishTable.push_back({ "����","����","Shad","fish/Shad.png",100,20,30 });
	fishTable.push_back({ "����","����","Eel","fish/Eel.png",200,20,30 });

	// ����
	fishTable.push_back({ "����","����","Sunfish","fish/Sunfish.png",150,20,30 });

	/* -------------- �ļ� ------------- */
	// �κ�����
	addFishForBothWeathers(fishTable, "�ļ�", "Tuna", "fish/Tuna.png", 300, 20, 30);
	addFishForBothWeathers(fishTable, "�ļ�", "Red Mullet", "fish/Red Mullet.png", 150, 20, 30);
	addFishForBothWeathers(fishTable, "�ļ�", "Pike", "fish/Pike.png", 250, 20, 30);
	addFishForBothWeathers(fishTable, "�ļ�", "Sturgeon", "fish/Sturgeon.png", 1000, 20, 30);

	// ����
	fishTable.push_back({ "�ļ�","����","Catfish","fish/Catfish.png",300,20,30 });
	fishTable.push_back({ "�ļ�","����","Shad","fish/Shad.png",100,20,30 });
	fishTable.push_back({ "�ļ�","����","Red Snapper","fish/Red Snapper.png",200,20,30 });

	// ����
	fishTable.push_back({ "�ļ�","����","Pufferfish","fish/Pufferfish.png",100,20,30 });
	fishTable.push_back({ "�ļ�","����","Sunfish","fish/Sunfish.png",150,20,30 });

	/* -------------- �＾ ------------- */
	// �κ�����
	addFishForBothWeathers(fishTable, "�＾", "Anchovy", "fish/Anchovy.png", 50, 20, 30);
	addFishForBothWeathers(fishTable, "�＾", "Smallmouth Bass", "fish/Smallmouth Bass.png", 100, 20, 30);
	addFishForBothWeathers(fishTable, "�＾", "Salmon", "fish/Salmon.png", 200, 20, 30);
	addFishForBothWeathers(fishTable, "�＾", "Midnight Carp", "fish/Midnight Carp.png", 200, 20, 30);

	// ����
	fishTable.push_back({ "�＾","����","Catfish","fish/Catfish.png",300,20,30 });
	fishTable.push_back({ "�＾","����","Shad","fish/Shad.png",100,20,30 });
	fishTable.push_back({ "�＾","����","Red Snapper","fish/Red Snapper.png",200,20,30 });
	fishTable.push_back({ "�＾","����","Eel","fish/Eel.png",200,20,30 });
	fishTable.push_back({ "�＾","����","Walleye","fish/Walleye.png",200,20,30 });

	/* -------------- ���� ------------- */
	// �κ�����
	addFishForBothWeathers(fishTable, "����", "Tuna", "fish/Tuna.png", 300, 20, 30);
	addFishForBothWeathers(fishTable, "����", "Pike", "fish/Pike.png", 250, 20, 30);
	addFishForBothWeathers(fishTable, "����", "Herring", "fish/Herring.png", 80, 20, 30);
	addFishForBothWeathers(fishTable, "����", "Red Snapper", "fish/Red Snapper.png", 200, 20, 30);
	addFishForBothWeathers(fishTable, "����", "Squid", "fish/Squid.png", 150, 20, 30);
	addFishForBothWeathers(fishTable, "����", "Sturgeon", "fish/Sturgeon.png", 1000, 20, 30);

	// ����
	fishTable.push_back({ "����","����","Red Mullet","fish/Red Mullet.png",150,20,30 });

	/* -------------- �ļ� ------------- */
	// ��������
	addFishAllSeasonsAndWeathers(fishTable, "Trash", "fish/Trash.png", 0, 0, 0);			// ����
	addFishAllSeasonsAndWeathers(fishTable, "Paper", "fish/Paper.png", 0, 0, 0);			// ��ֽ
	addFishAllSeasonsAndWeathers(fishTable, "JoJa Cola", "fish/JoJa Cola.png", 0, 10, 15);  // JoJa����

	return fishTable;  // �����������ɵ���������
}
