// AudioManager.h
#ifndef __AUDIO_MANAGER_H__
#define __AUDIO_MANAGER_H__

#include "cocos2d.h"
#include "audio/include/AudioEngine.h"

class AudioManager {
public:
    static AudioManager* getInstance();
    bool init();

    // �������ֿ���
    void playBackgroundMusic(const std::string& filename, bool loop = true);
    void stopBackgroundMusic();
    void pauseBackgroundMusic();
    void resumeBackgroundMusic();

    // ��������
    void setBackgroundMusicVolume(float volume);
    float getBackgroundMusicVolume() const;

    // ״̬��ѯ
    bool isBackgroundMusicPlaying() const;

    // ������Ϣ
    void logAudioStatus();

private:
    AudioManager() : isInitialized(false), bgmId(cocos2d::experimental::AudioEngine::INVALID_AUDIO_ID),
        bgmVolume(0.8f), currentBGM("") {
    }
    virtual ~AudioManager();

    static AudioManager* instance;
    bool isInitialized;
    int bgmId;  // ��ǰ�������ֵ�ID
    float bgmVolume;  // ��ǰ����
    std::string currentBGM;  // ��ǰ���ŵ������ļ���
};

#endif // __AUDIO_MANAGER_H__