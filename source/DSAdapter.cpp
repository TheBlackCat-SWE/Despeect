/**
  * @author Riccardo Damiani
  * @version 1.0 27/05/18
  */

#include "DSAdapter.hpp"
#include "DSRelation.hpp"
#include <QDebug>


DSAdapter::DSAdapter() :
    error(S_SUCCESS),
    text(NULL),
    voice(NULL),
    riff(NULL),
    utt(NULL)
{}

void DSAdapter::initSpeect(s_logger* logger) {
    S_CLR_ERR(&error);
    error = speect_init(logger);
    S_CHK_ERR(&error, S_FAILURE, "initSpeect",
              "Failed to initialize Speect\n");
}

void DSAdapter::quitSpeect() {
    S_CLR_ERR(&error);
    error = speect_quit();
    S_CHK_ERR(&error, S_FAILURE, "quitSpeect",
              "Failed to gracefully close Speect\n");
}

void DSAdapter::loadText(const std::string& text) {
    S_CLR_ERR(&error);
    if(!text.empty())
        this->text = SObjectSetString(text.c_str(), &error);
    S_CHK_ERR(&error, S_CONTERR, "loadText",
              "Failed to load text\n");
}

void DSAdapter::loadVoice(const std::string& voice_conf_path) {
    S_CLR_ERR(&error);
    voice = s_vm_load_voice(voice_conf_path.c_str(), &error);
    S_CHK_ERR(&error, S_CONTERR, "loadVoice",
              "Failed to load %s voice\n", voice_conf_path);
}

void DSAdapter::loadPlugin(const std::string& plugin_path) {
    S_CLR_ERR(&error);
    riff = s_pm_load_plugin(plugin_path.c_str(), &error);
    S_CHK_ERR(&error, S_CONTERR, "loadPlugin",
              "Failed to load &s plugin\n", plugin_path);
}

DSAdapter *DSAdapter::createAdapter() {
    DSAdapter* ptr = new DSAdapter;

    ptr->initSpeect(s_logger_console_new(true));

    if(!ptr->hasError()) {
        ptr->loadPlugin("audio_riff.spi");
        return ptr;
    }
    else {
        delete ptr;
        return NULL;
    }
}

bool DSAdapter::saveOutputAudio(const std::string& audio_output_path) {
    // Public interface non-const calls should check for internal integrity
    if(hasError()) {
        S_WARNING(error, "saveOutputAudio", "The system was found to be"
                  " in an inconsistent state before this func call.\n"
                  "This may result in unexpected behaviour\n");
    }

    S_CLR_ERR(&error);
    const SObject* audio_obj = SUtteranceGetFeature(utt, "audio", &error);
    if(hasError()) {
        S_WARNING(error, "saveOutputAudio", "Failed to fetch audio object,"
                  " no audio feature found in the current utterance\n");
        S_CLR_ERR(&error);
        return false;
    }
    SObjectSave(audio_obj, audio_output_path.c_str(), "riff", &error);
    delete audio_obj;
    if(hasError()) {
        S_WARNING(error, "saveOutputAudio", "Failed to save audio object, "
                  "error using 'audio_riff' plugin\n");
        S_CLR_ERR(&error);
        return false;
    }
    return true;
}

bool DSAdapter::synthesize() {
    // Public interface non-const calls should check for internal integrity
    if(hasError()) {
        S_WARNING(error, "synthesize", "The system was found to be"
                  " in an inconsistent state before this func call.\n"
                  "This may result in unexpected behaviour\n");
    }
    // Checks whether loadInputText was called successfully
    if(!text) {
        S_WARNING(error, "synthesize", "Input text not loaded yet\n");
        S_CLR_ERR(&error);
        return false;
    }

    S_CLR_ERR(&error);
    execUttType("text");
    S_CHK_ERR(&error, S_CONTERR, "synthesize",
              "Failed to complete synthesis process\n");

    if(!hasError()) return true;
    else return false;
}

bool DSAdapter::execUttType(const std::string& utt_type) {
    // Public interface non-const calls should check for internal integrity
    if(hasError()) {
        S_WARNING(error, "execUttType", "The system was found to be"
                  " in an inconsistent state before this func call.\n"
                  "This may result in unexpected behaviour\n");
    }

    S_CLR_ERR(&error);
    if(!utt) {
        utt = SVoiceSynthUtt(voice, utt_type.c_str(), text, &error);
        S_CHK_ERR(&error, S_CONTERR, "execUttType",
                  "Failed to execute the utterance type %s\n",
                  utt_type.c_str());
    }
    else {
        SVoiceReSynthUtt(voice, utt_type.c_str(), utt, &error);
        S_CHK_ERR(&error, S_CONTERR, "execUttType",
                  "Failed to re-execute the utterance type %s\n",
                  utt_type.c_str());
    }

    if(!hasError()) return true;
    else return false;
}

bool DSAdapter::execUttProcList(const std::vector<std::string>& proc_list) {
    // Public interface non-const calls should check for internal integrity
    if(hasError()) {
        S_WARNING(error, "execUttProcList", "The system was found to be"
                  " in an inconsistent state before this func call.\n"
                  "This may result in unexpected behaviour\n");
    }
    // Checks whether loadInputText was called successfully
    if(!text) {
        S_WARNING(error, "execUttProcList", "Input text not loaded yet\n");
        S_CLR_ERR(&error);
        return false;
    }

    resetUtterance();
    // Creates and initializes a new utterance if needed
    if(!utt) {
        S_CLR_ERR(&error);
        utt = S_NEW(SUtterance, &error);
        if(S_CHK_ERR(&error, S_CONTERR, "execUttProcList",
                     "Failed to create new utterance\n"))
            return false;
        S_CLR_ERR(&error);
        SUtteranceInit(&utt, voice, &error);
        if(S_CHK_ERR(&error, S_CONTERR, "execUttProcList",
                     "Failed to initialize new utterance\n"))
            return false;
        //set the text of the utterance that speect will use to the one of the configuration
        SUtteranceSetFeature(utt,"input",text, &error);
    }

    for(auto&& utt_proc_key : proc_list) {
        if(!execUttProc(utt_proc_key))
            return false;
    }
    return true;
}

//FIXME Come mai non mi serve il testo?
bool DSAdapter::execUttProc(const std::string& utt_proc_key) {
    // Public interface non-const calls should check for internal integrity
    if(hasError()) {
        S_WARNING(error, "execUttProc", "The system was found to be"
                  " in an inconsistent state before this func call.\n"
                  "This may result in unexpected behaviour\n");
    }
    // Checks whether loadInputText was called successfully
    if(!text) {
        S_WARNING(error, "execUttProc", "Input text not loaded yet\n");
        S_CLR_ERR(&error);
        return false;
    }

    // Creates and initializes a new utterance if needed
    if(!utt) {
        S_CLR_ERR(&error);
        utt = S_NEW(SUtterance, &error);
        if(S_CHK_ERR(&error, S_CONTERR, "execUttProc",
                     "Failed to create new utterance\n"))
            return false;
        S_CLR_ERR(&error);
        SUtteranceInit(&utt, voice, &error);
        if(S_CHK_ERR(&error, S_CONTERR, "execUttProc",
                     "Failed to initialize new utterance\n"))
            return false;
        //set the text of the utterance that speect will use to the one of the configuration
        SUtteranceSetFeature(utt,"input",text, &error);
    }

    S_CLR_ERR(&error);

    const SUttProcessor* utt_proc = SVoiceGetUttProc(voice, utt_proc_key.c_str(), &error);

    if(S_CHK_ERR(&error,
                 S_CONTERR,
                 "execUttProc",
                 "Failed to retrieve %s utterance type key\n",
                 utt_proc_key.c_str()))
    {
        return false;
    }

    SUttProcessorRun(utt_proc, utt, &error);

   if(S_CHK_ERR(&error,
                S_CONTERR,
                "execUttProc",
                "Failed to run %s utterance processor on current utterance\n",
                utt_proc_key.c_str()))
   {
        return false;
   }

    return true;
}

bool DSAdapter::resetUtterance() {
    // Public interface non-const calls should check for internal integrity
    if(hasError()) {
        S_WARNING(error, "resetUtterance", "The system was found to be"
                  " in an inconsistent state before this func call.\n"
                  "This may result in unexpected behaviour\n");
    }

    if(utt != NULL) {
        S_DELETE(utt, "resetUtterance", &error);
        delete utt;
        return true;
    }
    return false;
}

bool DSAdapter::hasError() const {
    return error != S_SUCCESS;
}

std::string DSAdapter::getText() const {
    s_erc error = S_SUCCESS;
    std::string str = std::string(SObjectGetString(text, &error));
    S_CHK_ERR(&error, S_CONTERR, "getText",
              "Failed to get text string\n");
    return str;
}

std::map<std::string, std::string> DSAdapter::getVoiceInfos() const {
    s_erc error = S_SUCCESS;

    std::map<std::string, std::string> map;

    std::string name(SVoiceGetName(voice, &error));
    S_CHK_ERR(&error, S_CONTERR, "getVoiceInfos",
              "Failed to get voice name info\n");
    if(error == S_SUCCESS) map[std::string("name")] = name;

    std::string desc(SVoiceGetDescription(voice, &error));
    S_CHK_ERR(&error, S_CONTERR, "getVoiceInfos",
              "Failed to get voice description info\n");
    if(error == S_SUCCESS) map[std::string("description")] = desc;

    std::string gender(SVoiceGetGender(voice, &error));
    S_CHK_ERR(&error, S_CONTERR, "getVoiceInfos",
              "Failed to get voice gender info\n");
    if(error == S_SUCCESS) map[std::string("gender")] = gender;

    std::string lang(SVoiceGetLanguage(voice, &error));
    S_CHK_ERR(&error, S_CONTERR, "getVoiceInfos",
              "Failed to get voice language info\n");
    if(error == S_SUCCESS) map[std::string("language")] = lang;

    std::string lang_code(SVoiceGetLangCode(voice, &error));
    S_CHK_ERR(&error, S_CONTERR, "getVoiceInfos",
              "Failed to get voice language code info\n");
    if(error == S_SUCCESS)
        map[std::string("lang_code")] = lang_code;

    const s_version* ver = SVoiceGetVersion(voice, &error);
    auto version =
            std::string(int(ver->major) + "." + int(ver->minor));
    S_CHK_ERR(&error, S_CONTERR, "getVoiceInfos",
              "Failed to get voice version info\n");
    if(error == S_SUCCESS) map[std::string("version")] = version;

    S_DELETE(ver, "getVoiceInfos", &error);
    S_CHK_ERR(&error, S_CONTERR, "getVoiceInfos",
              "Failed to get voice infos\n");
    return map;
}

std::vector<std::string> DSAdapter::getDataList() const {
    s_erc error = S_SUCCESS;

    SList* list = SVoiceGetDataKeys(voice, &error);
    S_CHK_ERR(&error, S_CONTERR, "getDataList",
              "Failed to retrieve data keys\n");
    std::vector<std::string> std_list(toStdList(list));

    S_DELETE(list, "getDataList", &error);
    return std_list;
}

std::vector<std::string> DSAdapter::getUttTypeList() const {
    s_erc error = S_SUCCESS;

    SList* list = SVoiceGetUttTypesKeys(voice, &error);
    S_CHK_ERR(&error, S_CONTERR, "getUttTypeList",
              "Failed to retrieve utterance type keys\n");
    std::vector<std::string> std_list(toStdList(list));

    S_DELETE(list, "getUttTypeList", &error);
    return std_list;
}

std::map<std::string, std::vector<std::string>> DSAdapter::getUttTypeDict() const {
    s_erc error = S_SUCCESS;

    std::map<std::string, std::vector<std::string>> utt_type_dict;

    std::vector<std::string> utt_type_list(getUttTypeList());
    for(auto&& utt_type : utt_type_list) {
        const SList* tmp =
                SVoiceGetUttType(voice, utt_type.c_str(), &error);
        utt_type_dict[utt_type] = toStdList(tmp);
    }

    S_CHK_ERR(&error, S_CONTERR, "getUttTypeDict",
              "Failed to retrieve utterance type dictionary\n");
    return utt_type_dict;
}

std::vector<std::string> DSAdapter::getUttProcList() const {
    s_erc error = S_SUCCESS;

    SList* list = SVoiceGetUttProcKeys(voice, &error);
    S_CHK_ERR(&error, S_CONTERR, "getUttProcList",
              "Failed to retrieve utterance processor keys\n");
    std::vector<std::string> std_list(toStdList(list));

    S_DELETE(list, "getUttProcList", &error);
    return std_list;
}

std::vector<std::string> DSAdapter::getFeatList() const {
    s_erc error = S_SUCCESS;

    SList* list = SVoiceGetFeatureKeys(voice, &error);
    S_CHK_ERR(&error, S_CONTERR, "getFeatList",
              "Failed to retrieve feature keys\n");
    std::vector<std::string> std_list(toStdList(list));

    S_DELETE(list, "getFeatList", &error);
    return std_list;
}

std::vector<std::string> DSAdapter::getFeatProcList() const {
    s_erc error = S_SUCCESS;

    SList* list = SVoiceGetFeatProcKeys(voice, &error);
    S_CHK_ERR(&error, S_CONTERR, "getFeatProcList",
              "Failed to retrieve feature processor keys\n");
    std::vector<std::string> std_list(toStdList(list));

    S_DELETE(list, "getFeatProcList", &error);
    return std_list;
}

std::vector<std::string> DSAdapter::getRelList() const {
    s_erc error = S_SUCCESS;

    SList* list = SUtteranceRelationsKeys(utt, &error);
    S_CHK_ERR(&error, S_CONTERR, "getRelList",
              "Failed to retrieve utterance relation keys\n");
    std::vector<std::string> std_list(toStdList(list));

    S_DELETE(list, "getRelList", &error);
    return std_list;
}

std::vector<std::string> DSAdapter::getUttFeatList() const {
    s_erc error = S_SUCCESS;

    SList* list = SUtteranceFeatKeys(utt, &error);
    S_CHK_ERR(&error, S_CONTERR, "getUttFeatList",
              "Failed to retrieve utterance feature keys\n");
    std::vector<std::string> std_list(toStdList(list));

    S_DELETE(list, "getUttFeatList", &error);
    return std_list;
}

DSRelation *DSAdapter::getRel(const std::string &rel) const {
    s_erc error = S_SUCCESS;

    const SRelation* tmp = SUtteranceGetRelation(utt, rel.c_str(), &error);
    S_CHK_ERR(&error, S_CONTERR, "getRel",
              "Failed to extract &s relation from internal utterance\n", rel);
    if(error == S_SUCCESS) return new DSRelation(tmp);
    else return NULL;
}

std::vector<std::string> DSAdapter::toStdList(const SList* spct_list) {
    s_erc error = S_SUCCESS;

    std::vector<std::string> std_list;

    SIterator* itr = S_ITERATOR_GET(spct_list, &error);
    S_CHK_ERR(&error, S_CONTERR, "toStdList",
              "Failed to get iterator to list, "
              "the list could be empty\n");
    if(error == S_SUCCESS) {
        while(itr) {
            const SObject* tmp = SIteratorObject(itr, &error);
            std_list.push_back(std::string(SObjectGetString(tmp, &error)));
            itr = SIteratorNext(itr);
        }
    }

    S_DELETE(itr, "toStdList", &error);
    S_CHK_ERR(&error, S_CONTERR, "toStdList",
              "Failed to convert spct_list to std_list\n");
    return std_list;
}

std::map<std::string, std::string> DSAdapter::toStdMap(const SMap* spct_map) {
    s_erc error = S_SUCCESS;

    std::map<std::string, std::string> std_map;

    std::vector<std::string> key_list =
            toStdList(SMapGetKeys(spct_map, &error));
    S_CHK_ERR(&error, S_CONTERR, "toStdMap",
              "Failed to get keys' list, "
              "the map could be empty\n");
    if(error == S_SUCCESS) {
        for(auto&& key : key_list) {
            const SObject* tmp =
                    SMapGetObject(spct_map, key.c_str(), &error);
            std_map[key] = SObjectGetString(tmp, &error);
        }
    }

    S_CHK_ERR(&error, S_CONTERR, "toStdMap",
              "Failed to convert spct_map to std_map\n");
    return std_map;
}

DSAdapter::~DSAdapter() {
    if(text) S_DELETE(text, "~DSAdapter", &error);
    if(voice) S_DELETE(voice, "~DSAdapter", &error);
    if(riff) S_DELETE(riff, "~DSAdapter", &error);
    // WHY DOES IT MAKE THE APPLICATION CRASH?? JESUS
    //if(utt) S_DELETE(utt, "~DSAdapter", &error);
    //MEMORY LEAK HERE

    quitSpeect();
}
