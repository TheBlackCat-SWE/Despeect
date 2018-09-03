/**
  * @author Riccardo Damiani
  * @version 1.4 26/05/18 (Additions: getRel func)
  */

#ifndef DSADAPTER_H
#define DSADAPTER_H

#include "speect.h"
#include <string>
#include <vector>
#include <map>

#include <QMessageBox>
#include <QFileDialog>

class DSRelation;


/**
 * @brief The DSAdapter class
 * This class acts as a wrapper over the speect API, allowing developers to
 * access the engine and all (hopefully, someday!) the TTS functionalities
 * available.
 */
class DSAdapter {
private:
    s_erc error;
    SObject* text;
    SVoice* voice;
    SPlugin* riff;
    SUtterance* utt;
protected:
    DSAdapter();
    /**
     * @brief initSpeect
     * Initialize the Speect Engine. This function must be called
     * before any other Speect Engine API calls.
     * @param logger Logger where messages should be written to. If #NULL,
     * then messages of error type will be written to @c stderr if the
     * build type (@c CMAKE_BUILD_TYPE) is @c Debug, otherwise no messages
     * will be logged.
     */
    void initSpeect(s_logger* logger);
    /**
     * @brief loadPlugin
     * Loads a plug-in from the given path. If the plug-in at the given
     * path has already been loaded, then the plug-in library's reference
     * counter is increased and a pointer to the loaded library is set in
     * the returned plug-in. This reduces the need for multiple redundant
     * calls. If the given path does not include any path separators (just
     * a file name) then the path is concatenated with the default plug-in
     * path.
     * @param plugin_path
     */
    void loadPlugin(const std::string& plugin_path);
public:
    /**
     * @brief createDSAdapter
     * Uses Named Constructor Idiom for 2-fase construction permitting
     * the halt of the construction if some component fails to initialize,
     * and to return a null pointer to the caller.
     * @return A pointer to the @c DSAdapter created, if the construction has
     * not encountered any errors, NULL otherwise.
     * @note The caller is responsible for the memory of the returned
     * @c DSAdapter.
     */
    static DSAdapter* createAdapter();
    /**
     * @brief quitSpeect
     * Requests a graceful shutdown of the Speect Engine.
     */
    void quitSpeect();
    /**
     * @brief loadText
     * Creates an @c SObject (of type @c SString) containing the external
     * input text and references it with a private pointer variable.
     * @param text The input text string to be loaded.
     */
    void loadText(const std::string& text);
    /**
     * @brief loadVoice
     * Loads a voice from the given path, making the speect internal
     * engine able to retrieve all the plugins related to the voice.
     * @param voice_conf_path The path where the voice.json can be
     * found.
     */
    void loadVoice(const std::string& voice_conf_path);

    // TODO loadInputTextFile

    // --------------------------- DANGER ZONE ---------------------------

    /**
     * @brief synthesize
     * Synthesizes the provided text using the "text" utterance type
     * pipeline.
     * @return True on success, false otherwise.
     */
    // FIXME Currently broken
    bool synthesize();
    /**
     * @brief saveOutputAudio
     * Missing description
     * @param audio_output_path The path for the audio file to be written to.
     */
    bool saveOutputAudio(const std::string& audio_output_path);
    // TODO saveUtterance

    /**
     * @brief execUttType
     * Is able to execute any utterance type (available for the given voice
     * configuration), creating a resulting #SUtterance, and storing a
     * pointer to be able to fetch it later on. It is also able to re-execute
     * utterance type onto the stored utterance multiple times.
     * @param utt_type The key of the utterance type as registered in the
     * @c SVoice @c uttTypes container.
     */
    bool execUttType(const std::string& utt_type);

    // TODO Add more manual controls to change internal speect state

    bool execUttProcList(const std::vector<std::string>& proc_list);
    bool execUttProc(const std::string& utt_proc_key);
    bool resetUtterance();

    // --------------------------- DANGER ZONE ---------------------------

    /**
     * @brief hasError
     * Controls whether if there is an internal error of any kind
     * or not.
     * @return True or False depending on the presence of an error.
     */
    bool hasError() const;
    /**
     * @brief getText
     * Access to the text to be synthesized stored internally.
     * @return The previously loaded input text string.
     */
    std::string getText() const;

    // --------------------------- Voice Related ---------------------------

    /**
     * @brief getVoiceInfos
     * Retrieves all the general informations related to the currently
     * loaded voice.
     * @return A map container with a brief description of the info
     * associated to a description.
     */
    std::map<std::string, std::string> getVoiceInfos() const;
    /**
     * @brief getDataList
     * Get a list of the @a data keys in the voice.
     * @return A vector container with the data
     */
    std::vector<std::string> getDataList() const;
    /**
     * @brief getUttTypeList
     * Fetches from the voice configuration a list of all the utterance
     * types available to be executed.
     * @return A list of utterance type names.
     */
    std::vector<std::string> getUttTypeList() const;
    /**
     * @brief getUttTypeDict
     * Fetches from the voice configuration a dictionary made of std
     * vectors linking all the utterance types to the corresponding utterance
     * processors.
     * @return A dictionary composed of nested std vectors filled with utterance
     * type names associated with their utterance processor names.
     */
    std::map<std::string, std::vector<std::string>> getUttTypeDict() const;
    /**
     * @brief getUttProcList
     * Fetches from the voice configuration a list of all the utterance
     * processors available to be executed.
     * @return A list of utterance processor names.
     */
    std::vector<std::string> getUttProcList() const;
    /**
     * @brief getFeatList
     * Gets all the feature names associated with the current voice.
     * @return A list of the @a feature keys in the voice.
     */
    std::vector<std::string> getFeatList() const;
    /**
     * @brief getFeatProcList
     * Gets all the feature processors available to be used with the current
     * voice.
     * @return A list containing all the feature processors' names.
     */
    std::vector<std::string> getFeatProcList() const;

    // --------------------------- Feature Related ---------------------------
    SObject* execFeatProcessor(const char* key, const SItem* item);

    // ------------------------- Utterance Related -------------------------

    /**
     * @brief getRelList
     * Gets the relation names obtained from the utterance in the current state.
     * @return The keys of the relations.
     */
    std::vector<std::string> getRelList() const;
    /**
     * @brief getUttFeatList
     * Gets all the features obtained from the utterance in the current state.
     * @return The keys of the features.
     */
    std::vector<std::string> getUttFeatList() const;
    /**
     * @brief getRel
     * Creates a read-only DSRelation object that wraps all the SRelation's
     * functionalities that do not alter the internal utterance state.
     * @param rel The name of the relation belonging to the utterance
     * @return A DSRelation pointer or null if it is impossible to create one.
     * @note The caller is responsible for the memory of the returned
     * @c DSRelation object.
     */
    DSRelation* getRel(const std::string& rel) const;


    /**
     * @brief nullUtterance
     * return true iff utterance is NULL, false otherwise
     *
     */
    bool nullUtterance() const;

    /**
     * @brief exportUtterance
     * @param path The path where to export the utterance
     *
     */
    void exportUtterance(std::string path) const;

    /**
     * @brief importUtterance
     * @param path The path from where to import the utterance
     *
     */
    void importUtterance(std::string path);

    // ------------------------- Utterance Related -------------------------

    /**
     * @brief toStdList
     * Converts a @c SList speect vector into a c++ sequential container
     * of strings
     * @param spct_list The List as an @c SList @c Object pointer
     * @return The List as a @c vector<string> c++ stdlib object
     */
    static std::vector<std::string> toStdList(const SList* spct_list);
    /**
     * @brief toStdMap
     * Converts an @c SMap speect map into a c++ associative container
     * of strings
     * @param spct_map The Map as an @c SMap @c SObject pointer
     * @return The Map as a @c map<string, string> c++ stdlib object
     */
    static std::map<std::string, std::string> toStdMap(const SMap* spct_map);

    ~DSAdapter();

    s_erc getError();
};

#endif // DSADAPTER_H
