#include <gtest/gtest.h>
#include "../header/DSAdapter.hpp"


#include <string>

using std::string;


TEST(DSAdapter, CorrectTextLoad) {
    DSAdapter* b=DSAdapter::createAdapter();
    string voice_path("../../Despeect/SpeectLib/voices/cmu_arctic_slt/voice.json");
    b->loadVoice(voice_path);
    b->loadText("I'm a test");
    EXPECT_EQ(b->getText(),"I'm a test");
}


