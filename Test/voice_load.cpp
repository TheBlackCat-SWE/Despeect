#include <gtest/gtest.h>
#include "../header/DSAdapter.hpp"



#include <string>

using std::string;


TEST(DSAdapter, FailedVoiceLoad) {
    DSAdapter* a=DSAdapter::createAdapter();
    string voice_path("test.json");
    a->loadVoice(voice_path);
    a->loadText("I'm a test");
    EXPECT_EQ(a->execUttType("text"),0);
}

TEST(DSAdapter, CorrectVoiceLoad) {
    DSAdapter* b=DSAdapter::createAdapter();
    string voice_path("../../Despeect/SpeectLib/voices/cmu_arctic_slt/voice.json");
    b->loadVoice(voice_path);
    b->loadText("I'm a test");
    EXPECT_EQ(b->execUttType("text"),1);
}




