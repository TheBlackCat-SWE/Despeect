#include <gtest/gtest.h>
#include "../header/DSAdapter.hpp"

#include <string>

using std::string;





TEST(DSAdapter, FailedVoiceLoad) {
    DSAdapter* a=DSAdapter::createAdapter();
    string voice_path("prova.json");
    a->loadVoice(voice_path);
    EXPECT_EQ(a->execUttType("prova"),0);
}

