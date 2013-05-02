#pragma once

#include "ofMain.h"

class VisConfig {
public:
    int quick_start;
    int fake_data;
    string ge_server_host;
    int ge_server_port;
    int ge_client_port;

    bool loadFile(string configPath){
        printf("loading config from %s\n", configPath.c_str());

        fstream file;

        file.open( configPath.c_str(), fstream::in );

        if( !file.is_open() ){
            printf("ERROR: Failed to open file: %s\n", configPath.c_str());
            return false;
        }

        string word;

        // read the file header
        file >> word;
        if( word != "VISUALIZER_CONFIG_FILE_V1.0" ){
            file.close();
            printf("ERROR: Failed to read file header!\n");
            return false;
        }

        // read GEServerHost
        file >> word;
        if( word != "GEServerHost:" ){
            file.close();
            printf("ERROR: Failed to read GEServerHost header!\n");
            return false;
        }
        file >> ge_server_host;

        // read GEServerPort
        file >> word;
        if( word != "GEServerPort:" ){
            file.close();
            printf("ERROR: Failed to read GEServerPort header!\n");
            return false;
        }
        file >> ge_server_port;

        // read GEClientPort
        file >> word;
        if( word != "GEClientPort:" ){
            file.close();
            printf("ERROR: Failed to read GEClientPort header!\n");
            return false;
        }
        file >> ge_client_port;

        // read QuickStart
        file >> word;
        if( word != "QuickStart:" ){
            file.close();
            printf("ERROR: Failed to read QuickStart header!\n");
            return false;
        }
        file >> quick_start;

        // read FakeData
        file >> word;
        if( word != "FakeData:" ){
            file.close();
            printf("ERROR: Failed to read FakeData header!\n");
            return false;
        }
        file >> fake_data;

        file.close();
        return true;
    }
};
