#include <string>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;



struct Participant //Участник
{
    string Name;
    int Id;
    double Score;
};

struct Contest //Конкурс
{
    vector<Participant> Participants; //Участники
    string Name;
    int MaxParticipants; //Макc. кол-во участников
    int PrizePlaceCount; //Кол-во призов
    int LoggedParticipantsCount; //Кол-во участников
    int Status;
    
    Contest(string name, int maxParticipants, int prizePlaceCount) // Новый
    {
        Name = name;
        Participants = vector<Participant>();
        MaxParticipants = maxParticipants;
        PrizePlaceCount = prizePlaceCount;
        LoggedParticipantsCount = 0;
        Status = 0;
    }
    
    Contest(string name, vector<Participant> participants, int maxParticipants, int prizePlaceCount)
    {
        Name = name;
        Participants = participants;
        MaxParticipants = maxParticipants;
        PrizePlaceCount = prizePlaceCount;
        LoggedParticipantsCount = participants.size();
        Status = 0;
    }
    
    bool TrySetScore(int id, double score) //Ставим баллы
    {
        if (Participants.size() < id)
        {
            return false;
        }
        else
        {
            Participants[id - 1].Score = score;
            return true;
        }
    }
    
    bool TryAddParticipant(string name) //Добавляем участника
    {
        if (Participants.size() == MaxParticipants)
            return false;
        else
        {
            Participant temp;
            temp.Id = Participants.size() + 1;
            temp.Name = name;
            temp.Score = 0;
            Participants.push_back(temp);
            return true;
        }
    }
};

bool compareParticipants(const Participant &first, const Participant &second)
{
    return first.Score > second.Score;
}


Contest ResumeContest(string name)
{
    ifstream file;
    file.open(name + "_participants.txt");
    int maxParticipants;
    int prizePlaceCount;
    file >> maxParticipants >> prizePlaceCount;
    vector<Participant> result;
    int counter = 1;
    while (!file.eof()) //Конец файла
    {
        Participant temp;
        getline(file, temp.Name);
        if (temp.Name != "")
        {
            temp.Id = counter++;
            temp.Score = 0;
            result.push_back(temp);
        }
    }
    file.close();
    return Contest(name, result, maxParticipants, prizePlaceCount);
}

bool IsContestExist(string name)
{
    ifstream file(name + "_participants.txt");
    return file.good();
}

void GetResults(vector<Participant> &participants)
{
    sort(participants.begin(), participants.end(), compareParticipants); //Сортировка по баллам
}


Contest StartNewContest(string name, int maxParticipants, int prizePlaceCount)
{
    return Contest(name, maxParticipants, prizePlaceCount);
}

void PrintParticipants(Contest contest)
{
    cout << "Участники: [номер участника] [имя участника] [количество очков]" << endl;
    
    for (int i = 0; i < contest.Participants.size(); i++)
    {
        cout << contest.Participants[i].Id << " " << contest.Participants[i].Name << " " << contest.Participants[i].Score << endl;
    }
}


void PrintResult(Contest contest)
{
    cout << "Название сореванования: " << contest.Name << endl;
    cout << "Количество участников " << contest.Participants.size() << endl;
    cout << "Максимальное количество участников в конкурсе " << contest.MaxParticipants << endl;
    cout << "Количество призовых мест " << contest.PrizePlaceCount << endl;
    cout << "Результаты конкурса: [место] [номер участника] [имя участника] [количество очков] " << endl;
    cout << "Победители:" << endl;
    for (int i = 0; i < min((int)contest.Participants.size(), contest.PrizePlaceCount); i++)
    {
        cout << i + 1 << " " << contest.Participants[i].Id << " " << contest.Participants[i].Name << " " << contest.Participants[i].Score << endl;
    }
    cout << "Остальные участники:" << endl;
    for (int i = contest.PrizePlaceCount; i < contest.Participants.size(); i++)
    {
        cout << i + 1 << " " << contest.Participants[i].Id << " " << contest.Participants[i].Name << " " << contest.Participants[i].Score << endl;
    }
}

void PrintResultToFile(Contest contest)
{
    ofstream file;
    file.open(contest.Name + "_result.txt");
    file << "Название сореванования: " << contest.Name << endl;
    file << "Количество участников " << contest.Participants.size() << endl;
    file << "Максимальное количество участников в конкурсе " << contest.MaxParticipants << endl;
    file << "Количество призовых мест " << contest.PrizePlaceCount << endl;
    file << "Результаты конкурса: [место] [номер участника] [имя участника] [количество очков]"<<endl;
    file << "Победители:" << endl;
    for (int i = 0; i < min((int)contest.Participants.size(), contest.PrizePlaceCount); i++)
    {
        file << contest.Participants[i].Id << " " << contest.Participants[i].Name << " " << contest.Participants[i].Score << endl;
    }
    file << "Остальные участники:" << endl;
    for (int i = contest.PrizePlaceCount; i < contest.Participants.size(); i++)
    {
        file << contest.Participants[i].Id << " " << contest.Participants[i].Name << " " << contest.Participants[i].Score << endl;
    }
    file.close();
}

void PrintParticipantsToFile(Contest contest)
{
    if (!IsContestExist(contest.Name)) // Проверка на налич файла
    {
        ofstream f(contest.Name + "_participants.txt");
        f << contest.MaxParticipants << " " << contest.PrizePlaceCount << endl;
        f.close();
    }
    ofstream file;
    file.open(contest.Name + "_participants.txt", std::ios::app); // Добавляем в конец файла
    for (int i = contest.LoggedParticipantsCount; i < contest.Participants.size(); i++)
    {
        file << contest.Participants[i].Name << endl;
    }
    file.close();
}


int main()
{
    setlocale(LC_ALL, "Russian");
    Contest currentContest("", 0, 0);
    while (true)
    {
        string input;
        cout << "1 - Новый конкурс\n2 - Загрузить конкурс\n3 - Добавить участника\n4 - Вывести всех участников\n";
        cout << "5 - Начать конкурс\n6 - Установить балл участника\n7 - Завершить конкурс\n8 - Подвести итоги\n9 - Выход\n";
        char c;
        
        cin >> c;
        
        switch (c)
        {
            case '1':
                if (currentContest.MaxParticipants != 0)
                    PrintParticipantsToFile(currentContest);
                cout << "Введите [название нового конкурса] [максимальное кол-во участников] [кол-во призовых мест]" << endl;
                int maxParticipants, prizePlaces;
                cin >> input >> maxParticipants >> prizePlaces;
                currentContest = StartNewContest(input, maxParticipants, prizePlaces);
                cout << "Новый конкурс был создан" << endl;
                break;
            case '2':
                if (currentContest.MaxParticipants != 0)
                    PrintParticipantsToFile(currentContest);
                cout << "Введите конкурса, который вы хотите загрузить" << endl;
                cin >> input;
                if (!IsContestExist(input))
                {
                    cout << "Такой конкурс не найден" << endl;
                    currentContest = Contest("", 0, 0);
                }
                else
                {
                    currentContest = ResumeContest(input);
                    cout << "Конкурс загружен" << endl;
                }
                break;
            case '3':
                if (currentContest.MaxParticipants == 0)
                {
                    cout << "Нельзя добавить участника пока не выбран конкурс" << endl;
                    break;
                } else if (currentContest.Status != 0)
                {
                    cout << "Нельзя добавить участника в конкурс который уже был начат" << endl;
                    break;
                }
                cout << "Введите имя участника" << endl;
                cin >> input;
                if (currentContest.TryAddParticipant(input))
                    cout << "Участник был успешно добавлен" << endl;
                else
                    cout << "Превышено количество участников конкурса" << endl;
                break;
            case '4':
                if (currentContest.MaxParticipants == 0)
                    cout << "Конкурс не выбран" << endl;
                else
                    PrintParticipants(currentContest);
                break;
            case '5':
                if (currentContest.MaxParticipants == 0)
                    cout << "Конкурс не выбран" << endl;
                else if (currentContest.Status == 1)
                    cout << "Нельзя начать конкурс который уже был начат" << endl;
                else
                {
                    currentContest.Status = 1;
                    cout << "Конкурс начат" << endl;
                }
                break;
            case '6':
                if (currentContest.MaxParticipants == 0)
                    cout << "Конкурс не выбран" << endl;
                else if (currentContest.Status != 1)
                    cout << "Конкурс недоступен для проставки баллов участников" << endl;
                else
                {
                    PrintParticipants(currentContest);
                    cout << "Введите [номер участника] [балл]";
                    int n;
                    double score;
                    cin >> n >> score;
                    if (!currentContest.TrySetScore(n, score))
                        cout << "Участник с таким номером не найден" << endl;
                    else
                        cout << "Балл участника успешно установлен" << endl;
                }
                break;
            case '7':
                if (currentContest.MaxParticipants == 0)
                    cout << "Конкурс не выбран" << endl;
                else if (currentContest.Status != 1)
                    cout << "Невозможно завершить этот конкурс" << endl;
                else
                {
                    cout << "Конкурс завершён, можете подвести итоги" << endl;
                    currentContest.Status = 2;
                }
                break;
            case '8':
                if (currentContest.MaxParticipants == 0)
                    cout << "Конкурс не выбран" << endl;
                else if (currentContest.Status != 2)
                    cout << "Невозможно подвести итоги незавершенного конкурса" << endl;
                else
                {
                    GetResults(currentContest.Participants);
                    PrintResult(currentContest);
                    PrintResultToFile(currentContest);
                }
                break;
            case '9':
                if (currentContest.MaxParticipants != 0)
                    PrintParticipantsToFile(currentContest);
                return 0;
            default:
                cout << "Неизвестная команда" << endl;
        }
        
    }
    return 0;
}
