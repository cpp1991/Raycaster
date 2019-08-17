#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "gamemanager.h"
#include "initialscene.h"
#include <iostream>
#include <fstream>
#include<QtDebug>
#include <string>
#include <windows.h>
#include <locale>
#include <codecvt>


// forward declarations
std::vector<std::vector<int>>* load_map(const std::wstring& path);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    wchar_t pBuf[256];
    size_t len = sizeof(pBuf);
    int bytes = GetModuleFileName(NULL, pBuf, len);

    if(bytes == 0) {
        qDebug() << "working directory could not be found. exit program";
    }

     std::wstring ws{pBuf};

    // load map (located on same level as applciation and named 'map.txt')
    std::vector<std::vector<int>> *  map = load_map(ws);

    // create initial scene and let is handle key presses
    InitialScene scene{map};
    scene.setFlag(QGraphicsItem::ItemIsFocusable);
    scene.setFocus();

    // create view for all scenes and add initial scene to it
    QGraphicsView view{&scene};
    view.show();

    return a.exec();
}

std::vector<std::vector<int>> * load_map(const std::wstring& path)
{
    std::string temporary_string;
    std::string final_string;
    std::ifstream in_file;
    int horizontal_count = 0;
    int vertical_count = 0;

    //setup converter
    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;

    //use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
    std::string converted_str = converter.to_bytes( path );

    std::string final_path = converted_str.replace(converted_str.find("Game.exe"), sizeof("Game.exe") - 1, "Map.txt");

    qDebug() << "finaler pfad: " << final_path.data();

    qDebug() << "trying to load file";
    in_file.open(final_path);
    //in_file.open ("C:/Users/B.Nuding/Desktop/testing.txt");
    if(in_file.is_open()) {
        qDebug() << "file is open, reading in...";
        while(!in_file.eof()) {
            horizontal_count++;

            getline(in_file,temporary_string); // Saves the line in STRING.

            final_string += temporary_string;
        }
        in_file.close();
        vertical_count = (int) final_string.size() / horizontal_count;
        qDebug() << "successfully read in file";
    }

    std::vector<std::vector<int>> *  map = new std::vector<std::vector<int>>{};

    int counter = 0;
    for (int horizontal = 0; horizontal < horizontal_count; ++horizontal) {
        std::vector<int> temp_vector{};
        for(int vertical = 0; vertical < vertical_count; ++vertical) {
            temp_vector.push_back(final_string.at(counter) - 48);
            counter++;
        }
        map->push_back(temp_vector);
    }

    return map;
}

