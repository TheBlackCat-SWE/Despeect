#include <gtest/gtest.h>
#include "../header/DSAdapter.hpp"

#include <string>

using std::string;
string voice_path("/home/davide/Desktop/TBC-Despeect-master/SpeectLib/voices/meraka_lwazi2_alta/voice.json");
DSAdapter* a=DSAdapter::createAdapter();


const char *actualValTrue  = "hello gtest";
const char *actualValFalse = "hello world";
const char *expectVal      = "hello gtest";

TEST(StrCompare, CStrEqual) {
    EXPECT_STREQ(expectVal, actualValTrue);
}

TEST(StrCompare, CStrNotEqual) {
    EXPECT_STREQ(expectVal, actualValFalse);
}
