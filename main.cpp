#include <iostream>
#include <ctime>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <iomanip>
#include <numeric>

// Storing different titles;
std::vector<std::string> _titles;

std::string csv_at(std::string path, int row, int col, bool h = true){
    std::ifstream csv;
    csv.open(path);
    std::string line;
    if (h)
        std::getline(csv, line);
    for (int i=0; i<row; i++)
        std::getline(csv, line);
    std::stringstream ss;
    std::string word;
    ss << line;
    for (int j=0; j<col; j++){
        std::getline(ss, word, ',');
    }
    return word;
}

int view_text(std::string path){
    std::ifstream file;
    file.open(path);
    std::string line;
    int row = 0;
    while (std::getline(file, line)){
        row++;
        std::cout << std::left << std::setw(3) << row <<
            line << std::endl;
    }
    return row;
}

void edit_text(std::string path, int row, std::string replace_as){
    std::ifstream text;
    text.open(path);
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(text, line))
        lines.push_back(line);
    text.close();
    lines.at(row-1) = replace_as;
    std::ofstream out;
    out.open(path, std::ios::out);
    for (int i=0; i<lines.size(); i++)
        out << lines.at(i) << "\n";
    out.close();
}

void edit_csv(std::string path, int row, int col, std::string replace_as, bool h=true){
    // Vector of headers, vector of the nth row.
    std::vector<std::string> lines;
    std::string header_line, content_line;
    // header or not? 
    std::ifstream csv;
    csv.open(path);
    if (h)
        std::getline(csv, header_line);
    while (std::getline(csv, content_line))
        lines.push_back(content_line);
    csv.close();
    // Choose the row-th line to edit
    content_line = lines.at(row-1);
    std::stringstream ss;
    ss << content_line;
    std::vector<std::string> cells_of_row;
    std::string word;
    while (std::getline(ss, word, ',')){
        cells_of_row.push_back(word);
    }
    // Replace the cell.
    cells_of_row.at(col-1) = replace_as;
    
    // Join vector, solution provided by Sasa Milenkovic, Neel Basu
    // https://stackoverflow.com/questions/5689003/how-to-implode-a-vector-of-strings-into-a-string-the-elegant-way
    content_line = std::accumulate(std::next(cells_of_row.begin()),
                        cells_of_row.end(),
                        cells_of_row[0],
                        [](std::string a, std::string b){
                            return a+","+b;
                        });
    // Replace row-th line.
    lines.at(row-1) = content_line;
    std::ofstream out;
    out.open(path, std::ios::out);
    out << header_line << "\n";
    for (int i=0; i<lines.size(); i++){
        out << lines.at(i) << "\n";
    }
    out.close();
}

// Read data, verify folders existence.
void prepare_data(){
    std::fstream titles_file;
    titles_file.open("titles.txt", std::ios::in);
    std::string title;
    if (titles_file){
        while (std::getline(titles_file, title)){
            _titles.push_back(title);
        }
    }
    titles_file.close();

    CreateDirectoryA("data", NULL);
}

void manual(){
    std::cout << "--- Begin Manual ---" << std::endl;
    std::cout << "This software served as a digital diary for you." << std::endl;
    std::cout << "You need to first create a title, for example \"Internship in Google\"" << std::endl;
    std::cout << "Then you can record your daily experiences in the title from time to time." << std::endl;
    std::cout << "More features coming on!" << std::endl;
    std::cout << "[Created with passion by Kelvin Hong]" << std::endl;
    std::cout << "--- End of Manual ---" << std::endl;
    return;
}

void add_title(){
    std::cout << "Please enter the title you want to create." << std::endl;
    std::string new_title;
    std::getline(std::cin, new_title);
    _titles.push_back(new_title);
    // Open txt file to record
    std::fstream fout;
    fout.open("titles.txt", std::ios::out | std::ios::app);
    if (!fout){
        std::cout << "\"titles.txt\" not found" << std::endl;
        return void();
    }
    else{
        fout << new_title << std::endl;
        fout.close();
    }
    std::cout << "New title <" << new_title << "> added" << std::endl;
}

void add_event(){
    int length = _titles.size();
    if (length == 0){
        std::cout << "There is no existing title. " <<
        "Please create title first." << std::endl;
        return;
    }
    else {
        std::cout << "Choose an index from " << length << " title(s)." << std::endl;
        for (int i=0; i<length; i++){
            std::cout << i+1 << ") " << _titles[i] << std::endl;
        }
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        std::string this_title;
        try {
            this_title = _titles.at(choice-1);
        }
        catch (const std::out_of_range& e){
            std::cout << "Invalid index. Try again." << std::endl;
            return;
        }
        time_t now = time(0);

        std::cout << "Enter a summary of the event." << std::endl;
        std::string summary;
        std::getline(std::cin, summary);

        std::cout << "Enter a full description of the event. " << std::endl;
        std::cout << "Place a blank line to finish." << std::endl;
        std::cout << "-----------------------------" << std::endl;
        std::ofstream out_event;
        std::stringstream ss;
        ss << now;
        out_event.open("data/" + this_title + "_" + ss.str() + ".txt");
        out_event << "<" << summary << ">\n\n" ;
        std::string s;
        while (std::getline(std::cin, s)){
            if (s.empty()){
                break;
            }
            else{
                out_event << s << std::endl;
            }
        }
        out_event.close();
        std::cout << "-----------------------------" << std::endl;
        
        std::ofstream out_events;
        out_events.open("events.csv", std::ios::app);
        out_events << this_title << "," << summary << "," << now << "," << now << std::endl;
        out_events.close();
        std::cout << "Event recorded." << std::endl;
    }
}

// After view titles, return the number of recorded events.
int view_titles(){
    std::ifstream csv;
    csv.open("events.csv");
    std::vector<std::string> titles{"Title"};
    std::vector<std::string> summaries{"EventSummary"};
    std::vector<std::string> seconds{"Second"};
    std::vector<std::string> datetimes{"DateTime"};
    std::vector<std::string> lastedits{"LastEdit"};
    std::cout << std::setw(5) << "Index" << 
        std::setw(30) << "Title" << 
        std::setw(20) << "EventSummary" <<
        std::setw(30) << "DateTime" << 
        std::setw(30) << "LastEdit" << std::endl;
    std::string line;
    std::getline(csv, line); // Ignore header
    int row = 0;
    std::string title, summary, second, datetime, lastedit_second, lastedit;
    while (std::getline(csv, line)){
        std::stringstream ss(line);
        std::getline(ss, title, ',');
        std::getline(ss, summary, ',');
        std::getline(ss, second, ',');
        time_t unixsecond = stoi(second);
        datetime = strtok(ctime(&unixsecond), "\n");
        std::getline(ss, lastedit_second, ',');
        time_t last_us = stoi(lastedit_second);
        lastedit = strtok(ctime(&last_us), "\n");
        titles.push_back(title);
        summaries.push_back(summary);
        seconds.push_back(second);
        datetimes.push_back(datetime);
        lastedits.push_back(lastedit);
        row = row+1;
        std::cout << std::setw(5) <<row <<
            std::setw(30) << title << 
            std::setw(20) << summary <<
            std::setw(30) << datetime <<
            std::setw(30) << lastedit <<  std::endl;
    }
    csv.close();
    return row;
}

// The setw() function should be set to flexible.
void view_events(){
    // View all, based on title.
    int row = view_titles();    
    std::cout << "Which event do you want to read?" << std::endl;
    std::cout << "Choose an index to read. Input 0 if you want to exit." << std::endl;
    int choice;
    std::cin >> choice;
    std::cin.ignore();
    if (choice > row){
        std::cout << "Index out of range. Please choose again." << std::endl;
        return;
    }
    std::stringstream ss;
    if (choice==0){
        return;
    }
    else{
        std::ifstream csv;
        csv.open("events.csv");
        std::string line;
        for (int i=0; i<=choice; i++){
            std::getline(csv, line);
        }
        ss.str(std::string());
        ss << line;
        std::string title, second;
        // Retrieve title and Unix-time.
        std::getline(ss, title, ',');
        std::getline(ss, second, ',');
        std::getline(ss, second, ',');
        ss.str(std::string());
        ss << "data/" << title << "_" << second << ".txt";
        // std::cout << ss.str();
        std::ifstream open_file;
        open_file.open(ss.str());
        std::string output;
        while (std::getline(open_file, output)){
            std::cout << output << std::endl;
        }
    }
}

void edit_events(){
    std::cout << "Select an event to edit." << std::endl;
    int row = view_titles();
    int choice;
    std::cin >> choice;
    std::cin.ignore();
    time_t now = time(0);
    std::stringstream ss;
    ss << now;
    if (choice>row){
        std::cout << "Index out of range. Please choose again." << std::endl;
        return ;
    }
    else {
        // Change summary
        std::cout << "Change summary? If yes input 1, If no input 0." << std::endl;
        edit_csv("events.csv", choice, 4, ss.str());
        int choice_s;
        std::cin >> choice_s;
        std::cin.ignore();
        std::string title = csv_at("events.csv", choice, 1);
        std::string unix_second = csv_at("events.csv", choice, 3);
        std::string textfile_loc = "data/"+title+"_"+unix_second+".txt"; 
        if (choice_s == 1){
            std::cout << "Enter new summary." << std::endl;
            std::string new_summary;
            std::getline(std::cin, new_summary);
            edit_csv("events.csv", choice, 2, new_summary);
            // Edit summary of event file
            edit_text(textfile_loc, 1, "<"+new_summary+">");
        }

        std::cout << "Edit content? If yes input 1, If no input 0." << std::endl;
        std::cin >> choice_s;
        std::cin.ignore();
        if (choice_s == 0){
            std::cout << "Edit file completed." << std::endl;
            return;
        }
        bool edit_mode = true;
        // Edit existing lines or append lines.
        std::cout << "You can edit existing lines first, then append new contents if you want." << std::endl;
        int row = view_text(textfile_loc);
        int edit_row;
        std::string line;
        // Edit mode
        while (edit_mode){
            std::cout << "Which line do you want to edit? Enter 0 to continue to append mode." << std::endl;
            std::cin >> edit_row;
            std::cin.ignore();
            if (edit_row == 0)
                edit_mode = false;
            else if (edit_row <= 2 | edit_row > row){
                std::cout << "You can't edit summary or append new contents here." << std::endl;
                continue;
            }
            else{
                std::cout << "Change line " << edit_row << " to:" << std::endl;
                std::getline(std::cin, line);
                edit_text(textfile_loc, edit_row, line);
            }
        }
        // Append mode
        std::cout << "This is the text so far. Proceed to append mode." << std::endl;
        row = view_text(textfile_loc);
        std::cout << "Append new contents. Place blank line to finish." << std::endl;
        std::ofstream textfile;
        textfile.open(textfile_loc, std::ios::app);
        while (true) {
            std::getline(std::cin, line);
            if (!line.empty())
                textfile << line << std::endl;
            else
                break;
        }
        textfile.close();
        std::cout << "Edit file completed." << std::endl;
    }
}


int main(){
    prepare_data();
    std::ifstream this_file;
    this_file.open("events.csv");
    if (!this_file){
        std::ofstream new_eventsfile;
        new_eventsfile.open("events.csv");
        new_eventsfile << "Title,EventSummary,Time,LastEdit\n";
        new_eventsfile.close();
    }
    

    std::cout << "Hi Kelvin Hong." << std::endl;
    std::cout << "Choose one option:" << std::endl;
    std::cout << "0. Manual" << std::endl;
    std::cout << "1. Create new titles." << std::endl;
    std::cout << "2. Record new event on existing title." << std::endl;
    std::cout << "3. View existing events." << std::endl;
    std::cout << "4. Edit existing events." << std::endl;
    int main_option;
    std::cin >> main_option;
    std::cin.ignore();
    if (main_option == 0){
        manual();
    }
    else if (main_option == 1){
        add_title();
    }
    else if (main_option == 2){
        add_event();
    }
    else if (main_option == 3){
        view_events();
    }
    else if (main_option == 4){
        edit_events();
    }
}
