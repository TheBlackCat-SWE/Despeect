#include <gtest/gtest.h>
#include "../header/DSAdapter.hpp"
#include "../header/DSRelation.hpp"
#include "../header/DSItem.hpp"

#include <string>

using std::string;


TEST(DSAdapter, FailedVoiceLoad) {
    DSAdapter* a=DSAdapter::createAdapter();
    string voice_path("prova.json");
    a->loadVoice(voice_path);
    a->loadText("Sono un test");
    EXPECT_EQ(a->execUttType("prova"),0);
}

TEST(DSAdapter, CorrectVoiceLoad) {
    DSAdapter* b=DSAdapter::createAdapter();
    string voice_path("/home/davide/Desktop/grwegremgl/Despeect/SpeectLib/voices/cmu_arctic_slt/voice.json");
    b->loadVoice(voice_path);
    b->loadText("Sono un test");
    EXPECT_EQ(b->execUttType("text"),1);
}


TEST(DSAdapter, CorrectRelationName) {
    DSAdapter* b=DSAdapter::createAdapter();
    string voice_path("/home/davide/Desktop/grwegremgl/Despeect/SpeectLib/voices/cmu_arctic_slt/voice.json");
    b->loadVoice(voice_path);
    b->loadText("Sono un test");
    b->execUttType("text");
    EXPECT_EQ(b->getRel("Token")->getName(),"Token");
}
