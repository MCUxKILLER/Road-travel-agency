#include <bits/stdc++.h>
#include <regex>
#include <cstring>
#include <cstdlib>
using namespace std;

const int INF = 1e9;
int cnt = 0;
vector<vector<int>> store;
vector<int> stCost;
// vector<string>cities={"Noida","Meerut","Jaipur","Surat","Rajkot","Haridwar","Dehradun","Lucknow","Vadodara","Goa"};

void userMenu(string name);

vector<string> cities = {"Noida", "Meerut", "Jaipur", "Surat", "Rajkot"};

vector<bool> visited(cities.size(), false);
vector<int> path;

struct Car
{
    string carname;
    int quantity;
    int charge;
};

struct driver{

    string name;
    int age;

};

vector<driver> dataDriver(string file_n){

    vector<driver> data;
    string dname;
    int dage;
    ifstream file;
    file.open(file_n);

    while(file>>dname>>dage){
        data.push_back({dname,dage});
    }

    return data;
}

vector<driver> Driver = dataDriver("drivers.txt");
vector<driver> availDriver = dataDriver("availableDrivers.txt");

int rows, cols;

vector<Car> putInVec()
{
    vector<Car> car;
    string carName;
    int qnt;
    int chrg;
    ifstream file;
    file.open("cars.txt");

    int i = 0;

    while (file >> carName >> qnt >> chrg)
    {

        car.push_back({carName, qnt, chrg});
    }

    file.close();
    return car;

}

vector<Car> car = putInVec();

int del(string carName)
{
    // Open the input file
    ifstream input("cars.txt");
    if (!input)
    {
        cerr << "Error: Failed to open input file." << endl;
        return 1;
    }

    // Open a temporary file for writing
    ofstream temp("temp.txt");
    if (!temp)
    {
        cerr << "Error: Failed to open temporary file." << endl;
        return 1;
    }

    // Read the selected car name from the user
    string selected_car = carName;

    // Loop over each line in the input file
    string line;
    while (getline(input, line))
    {
        // Parse the line into its components
        stringstream ss(line);
        string car_name;
        int quantity;
        double cost;
        ss >> car_name >> quantity >> cost;

        // If the car name matches the selected car, reduce its quantity by 1
        if (car_name == selected_car)
        {
            quantity--;
        }

        // Write the updated line to the temporary file
        temp << car_name << " " << quantity << " " << cost << endl;
    }

    // Close the input and temporary files
    input.close();
    temp.close();

    // Replace the input file with the temporary file
    if (remove("cars.txt") != 0)
    {
        cerr << "Error: Failed to remove input file." << endl;
        return 1;
    }
    if (rename("temp.txt", "cars.txt") != 0)
    {
        cerr << "Error: Failed to rename temporary file." << endl;
        return 1;
    }

    return 0;
}

void addDriverAvail(string name,int age){

    ofstream file;
    file.open("availableDrivers.txt",ios::app);

    file<<name<<" "<<age<<endl;

    file.close();

}

void delFromAvail(string name){

    int i;
    for(i=0; i<availDriver.size();i++){
        if(availDriver[i].name == name){
            break;
        }
    }
    availDriver.erase(availDriver.begin()+i);
    ofstream file;
    file.open("availableDrivers.txt");

    for(int j=0;j<availDriver.size();j++){
        file<<availDriver[j].name<<" "<<availDriver[j].age<<endl;
    }
    file.close();

}

int increaseQuan(string carName)
{
    // Open the input file
    ifstream input("cars.txt");
    if (!input)
    {
        cerr << "Error: Failed to open input file." << endl;
        return 1;
    }

    // Open a temporary file for writing
    ofstream temp("temp.txt");
    if (!temp)
    {
        cerr << "Error: Failed to open temporary file." << endl;
        return 1;
    }

    // Read the selected car name from the user
    string selected_car = carName;

    // Loop over each line in the input file
    string line;
    while (getline(input, line))
    {
        // Parse the line into its components
        stringstream ss(line);
        string car_name;
        int quantity;
        double cost;
        ss >> car_name >> quantity >> cost;

        // If the car name matches the selected car, reduce its quantity by 1
        if (car_name == selected_car)
        {
            quantity++;
        }

        // Write the updated line to the temporary file
        temp << car_name << " " << quantity << " " << cost << endl;
    }

    // Close the input and temporary files
    input.close();
    temp.close();

    // Replace the input file with the temporary file
    if (remove("cars.txt") != 0)
    {
        cerr << "Error: Failed to remove input file." << endl;
        return 1;
    }
    if (rename("temp.txt", "cars.txt") != 0)
    {
        cerr << "Error: Failed to rename temporary file." << endl;
        return 1;
    }

    return 0;
}

vector<vector<int>> putData(int size, int &rows, int &cols)
{
    ifstream infile("matrix.txt");

    // Read the dimensions of the matrix
    infile >> rows >> cols;

    // Create a vector of vectors to hold the matrix
    vector<vector<int>> matrix(rows, vector<int>(cols));

    // Read each row of the matrix from the file
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            infile >> matrix[i][j];
        }
    }

    // Close the file
    infile.close();
    return matrix;
}

vector<vector<int>> edges = putData(cities.size(), rows, cols);

// Prints a path
void printPath(const vector<int> &path, const vector<vector<int>> &matrix, int &count, vector<int> &stCost)
{

    cout << "[" << count << "] ";
    count++;
    for (int i = 0; i < path.size() - 1; i++)
    {
        cout << cities[path[i]] << " -> ";
    }
    cout << cities[path.back()] << endl;

    // Calculate the cost of the path
    int cost = 0;
    for (int i = 0; i < path.size() - 1; i++)
    {
        cost += matrix[path[i]][path[i + 1]];
    }
    stCost.push_back(cost);
    cout << "    Distance: " << cost*10<<"Kms" << endl
         << endl;
}

// Finds all paths from the source to the destination using DFS
void findAllPaths(const vector<vector<int>> &matrix, int n, int m, int src, int dst, vector<bool> &visited, vector<int> &path, vector<vector<int>> &store, vector<int> &stCost)
{

    visited[src] = true;
    path.push_back(src);
    if (src == dst)
    {
        for (int i = 0; i < store.size(); i++)
        {
            if (store[i] == path)
            {
                return;
            }
        }
        store.push_back(path);
        printPath(path, matrix, cnt, stCost);
    }

    else
    {
        for (int neighbor = 0; neighbor < m; neighbor++)
        {
            if (matrix[src][neighbor] > 0 && !visited[neighbor])
            {
                findAllPaths(matrix, n, m, neighbor, dst, visited, path, store, stCost);
            }
        }
    }
    path.pop_back();
    visited[src] = false;
}

void dijkstra(vector<vector<int>> edges, int source, int n, int dest, vector<vector<int>> &store, vector<int> &stCost, int &count)
{

    vector<int> dist(n, INF);
    vector<int> prev(n, -1); // to keep track of the shortest path
    dist[source] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, source});

    while (!pq.empty())
    {
        int u = pq.top().second;
        pq.pop();

        for (int v = 0; v < n; v++)
        {
            if (edges[u][v] != 0 && dist[u] + edges[u][v] < dist[v])
            {
                dist[v] = dist[u] + edges[u][v];
                prev[v] = u; // update the previous vertex for v
                pq.push({dist[v], v});
            }
        }
    }

    if (dist[dest] == INF)
    {

        cout << "No path found from " << source << " to " << dest << endl;
    }

    else
    {

        // print the path in reverse order
        vector<int> path;
        int current = dest;

        while (current != -1)
        {
            path.push_back(current);
            current = prev[current];
        }
        cout << "[" << count << "] ";
        count++;
        int i;

        for (i = path.size() - 1; i >= 1; i--)
            cout << cities[path[i]] << " --> ";

        cout << cities[path[i]] << endl;
        cout << "    Distance: " << dist[dest]*10<<"Kms";
        stCost.push_back(dist[dest]);
        cout << endl
             << endl;
        reverse(path.begin(), path.end());
        store.push_back(path);
    }
}

void merge(vector<Car>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temporary arrays
    vector<Car> leftArr(n1);
    vector<Car> rightArr(n2);

    // Copy data to temporary arrays
    for (int i = 0; i < n1; i++)
        leftArr[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        rightArr[j] = arr[mid + 1 + j];

    // Merge the temporary arrays back into the main array
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (leftArr[i].charge <= rightArr[j].charge) {
            arr[k] = leftArr[i];
            i++;
        }
        else {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of leftArr[], if there are any
    while (i < n1) {
        arr[k] = leftArr[i];
        i++;
        k++;
    }

    // Copy the remaining elements of rightArr[], if there are any
    while (j < n2) {
        arr[k] = rightArr[j];
        j++;
        k++;
    }
}

// Merge sort function
void mergeSort(vector<Car>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        // Sort first and second halves
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        // Merge the sorted halves
        merge(arr, left, mid, right);
    }
}

bool validatePhoneNumber(string number)
{

    regex pattern("(\\+\\d{1,2}[- ]?)?\\d{10}");

    if (regex_match(number, pattern))
    {
        return true;
    }
    else
    {

        return false;
    }
}

bool checkemail(string email)
{
    const regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
    return regex_match(email, pattern);
}

bool checkPass(string password)
{
    // regex pattern for password validation
    regex pattern("(?=.*[a-z])(?=.*[A-Z])(?=.*[0-9])(?=.*[@#$%^&+=])(?=\\S+$).{8,}");

    // check if the password matches the pattern
    if (regex_match(password, pattern))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void registration()
{

    string name;
    string email;
    string phone;
    string password;
    string cnfpass;
    cout << endl;
    cout << "----------REGISTER YOURSELF----------" << endl
         << endl;
    cout << "Enter your username : ";
    cin >> name;
    cout << "Enter your email : ";
    cin >> email;

    while (checkemail(email) != 1)
    {

        cout << endl;
        cout << "Reenter email address" << endl;
        cout << "Enter your email : ";
        cin >> email;
    }
    cout << "Enter your phone number : ";
    cin >> phone;

    while (validatePhoneNumber(phone) != 1)
    {
        cout << endl;
        cout << "Invalid phone number" << endl;
        cout << "Enter your phone number : ";
        cin >> phone;
    }

    cout << "Enter your password : ";
    cin >> password;

    while (checkPass(password) != 1)
    {

        cout << endl;
        cout << "Reenter password" << endl;
        cout << "VALID PASSWORD : atleast 8 characters" << endl
             << "\t\t atleast one lower case" << endl
             << "\t\t atleast one upper case" << endl
             << "\t\t atleast one number" << endl
             << "\t\t atleast one special character" << endl
             << "\t\t no whitespaces" << endl
             << endl;
        cout << "Enter your password : ";
        cin >> password;
    }

    cout << "Confirm password : ";
    cin >> cnfpass;

    int length1 = password.length();
    int length2 = cnfpass.length();
    char *a = new char[length1 + 1];
    char *b = new char[length2 + 1];

    strcpy(a, password.c_str());
    strcpy(b, cnfpass.c_str());

    while (strcmp(a, b) != 0)
    {

        cout << endl;
        cout << "Re-confirm your password" << endl;
        cout << "Confirm password : ";
        cin >> cnfpass;
        strcpy(b, cnfpass.c_str());
    }
    cout << endl;

    fstream file;
    string filename = name + ".txt";
    file.open(filename, ios::out);

    if (file.is_open())
    {

        file << name << " " << email << " " << phone << " " << password << endl;
        file << "@" << endl;
    }

    file.close();

    ofstream ptr("AllUsers.txt", ios::app);
    if (ptr.is_open())
    {

        ptr << name << " " << email << " " << password << endl;
    }

    ptr.close();

    userMenu(name);
}

void login()
{
    string cName;
    string cEmail;
    string cPass;
    ifstream file;
    string mail, pass;
    int checkCount = 0;

    cout << endl;
    cout << "----------LOGIN----------" << endl
         << endl;
    cout << "Enter your Email Address : ";
    cin >> mail;

    while (checkemail(mail) != 1)
    {
        cout << endl;
        cout << "Reenter email address" << endl;
        cout << "Enter your email : ";
        cin >> mail;
    }

    cout << "Enter your password : ";
    cin >> pass;
    cout << endl;
    file.open("AllUsers.txt");

    while (!file.eof())
    {
        file >> cName >> cEmail >> cPass;
        if (cEmail == mail)
        {
            checkCount++;
            if (cPass == pass)
            {

                cout << "Welcome " << cName << "!!! enjoy travelling!" << endl
                     << endl;
                userMenu(cName);
                break;
            }

            cout << "Incorrect email ID or password" << endl
                 << endl;
            login();
            break;
        }
    }

    if (checkCount == 0)
    {
        char c;
        cout << "Seems you don't have an account here" << endl;
        cout << "Would you like to Register?(y/n)";
        cin >> c;

        if (c == 'y')
        {

            registration();
        }
        else if (c == 'n')
        {

            cout << "IT WAS GOOD TO HAVE YOU HERE" << endl;
            exit(0);
        }
    }

    file.close();
}

int check(string name)
{

    cout << endl;
    cout << "----------BOOKINGS----------" << endl
         << endl;
    string filename = name + ".txt";

    // Open the file
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Failed to open file." << endl;
        return 1;
    }

    // Read the user details from the file
    string user_details;
    getline(file, user_details);

    // Read the bookings from the file
    vector<vector<int>> bookings;
    vector<int> costs;
    vector<string> carname;
    vector<string> driveName;
    vector<int>age;
    vector<int> carcost;
    string line;
    int k=0;

    while (getline(file, line))
    {

        // Check if line is the separator
        if (line == "@")
        {
            continue;
        }
        k++;
        // Parse the indexes and add to bookings vector
        stringstream ss(line);
        vector<int> booking;
        int index;
        while (ss >> index)
        {
            if (index == -1)
            {
                break;
            }
            booking.push_back(index);
        }
        bookings.push_back(booking);
        // Parse the cost and add to costs vector
        int cost;
        ss >> cost;
        costs.push_back(cost);
        int sep;
        ss >> sep;
        int car_cost;
        string car_name;
        string dname;
        int dage;
        ss >> car_name >> car_cost>>dname >> dage;
        carname.push_back(car_name);
        carcost.push_back(car_cost);
        driveName.push_back(dname);
        age.push_back(dage);

    }

    if(k==0){
        cout<<"\tNo BooKings Yet!!"<<endl<<endl;
        userMenu(name);
        return 0;
    }
    // Close the file
    file.close();
    cout << "YOUR BOOKINGS : " << endl
         << endl;
    int l, m;
    for (l = 0; l < bookings.size(); l++)
    {
        cout << "Booking " << l + 1 << ": ";
        for (m = 0; m < bookings[l].size() - 1; m++)
        {
            cout << cities[bookings[l][m]] << "-->";
        }
        cout << cities[bookings[l][m]] << " ";
        cout << "\n\t   Distance:" << costs[l]*10 << "Kms \n\t   Car_booked:" << carname[l] << " \n\t   Journey_Cost:Rs." << carcost[l] << "\n\t   Driver's Name:"<<driveName[l]<<"\n\t   Driver's Age:"<<age[l]<<endl<<endl;
    }

    userMenu(name);
}

int cancel(string name)
{

    // Get the file name from user
    cout << endl;
    cout << "----------CANCELLATION----------" << endl
         << endl;
    string filename = name + ".txt";

    // Open the file
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Failed to open file." << endl;
        return 1;
    }

    // Read the user details from the file
    string user_details;
    getline(file, user_details);

    // Read the bookings from the file
    vector<vector<int>> bookings;
    vector<int> costs;
    vector<string> carname;
    vector<int> carcost;
    vector<string> driveName;
    vector<int>age;
    string line;
    int k=0;

    while (getline(file, line))
    {

        // Check if line is the separator
        if (line == "@")
        {
            continue;
        }
        k++;
        // Parse the indexes and add to bookings vector
        stringstream ss(line);
        vector<int> booking;
        int index;
        while (ss >> index)
        {
            if (index == -1)
            {
                break;
            }
            booking.push_back(index);
        }
        bookings.push_back(booking);
        // Parse the cost and add to costs vector
        int cost;
        ss >> cost;
        costs.push_back(cost);
        int sep;
        ss >> sep;
        int car_cost,dage;
        string car_name,dname;
        ss >> car_name >> car_cost>>dname>>dage;
        carname.push_back(car_name);
        carcost.push_back(car_cost);
        driveName.push_back(dname);
        age.push_back(dage);
    }

    if(k==0){
        cout<<"\tNo Bookings to cancel"<<endl<<endl;
        userMenu(name);
        return 0;
    }

    // Close the file
    file.close();

    // Print the user details and bookings
    int l, m;
    cout << "User details: " << user_details << endl<<endl;
    for (l = 0; l < bookings.size(); l++)
    {
        cout << "Booking " << l + 1 << ": ";
        for (m = 0; m < bookings[l].size() - 1; m++)
        {
            cout << cities[bookings[l][m]] << "-->";
        }
        cout << cities[bookings[l][m]] << " ";
        cout << "\n\t   Distance:" << costs[l]*10 << "Kms \n\t   Car_booked:" << carname[l] << " \n\t   Journey_Cost:Rs." << carcost[l] << "\n\t   Driver's Name:"<<driveName[l]<<"\n\t   Driver's Age:"<<age[l]<<endl<<endl;
    }

    // Get the index of the booking to delete
    int index_to_delete;
    cout<<endl;
    cout << "Enter the index of the booking to delete: ";
    cin >> index_to_delete;
    if (index_to_delete < 1 || index_to_delete > bookings.size())
    {
        cerr << "Invalid index." << endl;
        return 1;
    }

    string storeCarName = carname[index_to_delete - 1];

    addDriverAvail(driveName[index_to_delete - 1],age[index_to_delete - 1]);
    availDriver.push_back({driveName[index_to_delete - 1],age[index_to_delete - 1]});
    // Remove the selected booking from the vectors
    bookings.erase(bookings.begin() + index_to_delete - 1);
    costs.erase(costs.begin() + index_to_delete - 1);
    carname.erase(carname.begin() + index_to_delete - 1);
    carcost.erase(carcost.begin() + index_to_delete - 1);
    driveName.erase(driveName.begin() + index_to_delete - 1);
    age.erase(age.begin() + index_to_delete - 1);

    // Write the updated data to the file
    ofstream outfile(filename);
    if (!outfile.is_open())
    {
        cerr << "Failed to open file." << endl;
        return 1;
    }

    // Write the user details to the file
    outfile << user_details << endl;
    outfile << "@" << endl;
    // Write the bookings to the file
    for (int i = 0; i < bookings.size(); i++)
    {

        for (int j = 0; j < bookings[i].size(); j++)
        {

            outfile << bookings[i][j] << " ";
        }

        outfile << "-1 " << costs[i] << " -2 " << carname[i] << " " << carcost[i] << " "<<driveName[i]<<" "<<age[i]<<endl;
    }

    // Close the file
    outfile.close();

    int modCars = increaseQuan(storeCarName);
    car.clear();
    car = putInVec();

    cout<<endl;
    cout<<"SUCCESSFULLY CANCELLED THE BOOKING...."<<endl;
    userMenu(name);
    return 0;
}

void booking(string name)
{
    store.clear();
    stCost.clear();
    cnt=0;
    cout << endl;
    // transform(name.begin(),name.end(),name.begin(),::toupper);
    cout << "-------------Begin your Journey here " << name << "-------------" << endl;
    int start;
    int end;
    int pathChoice;
    cout<<"Cities you can travel from and cities you can travel to : "<<endl;

    for(int i=0;i<cities.size();i++){
        cout<<"\t["<<i+1<<"] "<<cities[i]<<endl;
    }

    cout<<endl;
    cout << "Enter the Starting city of your journey (Bullet no. from list) : ";
    cin >> start;
    cout << "Enter the Destination city of your journey (Buller no. from list) : ";
    cin >> end;
    int src;
    int dest;

    // for (int i = 0; i < cities.size(); i++)
    // {

    //     if (start.compare(cities[i]) == 0)
    //     {
    //         src = i;
    //     }

    //     if (end.compare(cities[i]) == 0)
    //     {
    //         dest = i;
    //     }
    // }
    if(start<1 || end<1 || start>cities.size()||end>cities.size()){
        cout<<"Invalid City Index."<<endl;
        cout<<"RESTART THE PROCESS"<<endl;
        booking(name);
    }

    else{
        src = start-1;
        dest = end-1;
    }

    cout << endl;
    cout << "SHORTEST PATH : " << endl
         << endl;
    dijkstra(edges, src, cities.size(), dest, store, stCost, cnt);

    cout << "Other possible routes : " << endl
         << endl;
    findAllPaths(edges, rows, cols, src, dest, visited, path, store, stCost);

    cout << "Enter the path you would like for your travel :";
    cin >> pathChoice;

    cout << endl;
    int j;

    cout << endl;

    cout << "Showing You available Cars and their per Km cost  " << endl
         << endl;
    int carChoice;

    mergeSort(car,0,car.size()-1);

    for (int i = 0; i < car.size(); i++)
    {

        if (car[i].quantity != 0)
        {

            cout << "[" << i << "]"
                 << " " << car[i].carname << "  Cost: " << car[i].charge << endl;
        }
    }

    cout << "Which Car would you like to travel with : ";
    cin >> carChoice;

    char c;
    cout << endl;

    cout << "------------Reconfirming details------------" << endl
         << endl;
    cout << "Departure from : " << cities[src] << endl;
    cout << "Destination : " << cities[dest] << endl;
    cout << "Path you chose for : ";

    for (j = 0; j < store[pathChoice].size() - 1; j++)
    {

        cout << cities[store[pathChoice][j]] << " --> ";
    }

    int jrnCost = stCost[pathChoice] * 10 * car[carChoice].charge;

    cout << cities[store[pathChoice][j]] << endl;
    cout << "Car Chosen : " << car[carChoice].carname << endl;
    cout << "Cost of your journey will be : Rs." << jrnCost << endl;
    int randIndex = rand()%(availDriver.size());
    cout<<"Driver Assigned : "<<availDriver[randIndex].name << endl<<endl;
    cout << "Do you confirm your journey? (y/n) : ";
    cin >> c;
    cout << endl;

    if (c == 'y')
    {

        string filename = name + ".txt";
        ofstream file;
        file.open(filename, ios::app);

        for (j = 0; j < store[pathChoice].size() - 1; j++)
        {

            file << store[pathChoice][j] << " ";
        }

        int sep = -1;
        int carSep = -2;

        file << store[pathChoice][j] << " " << sep << " ";

        file << stCost[pathChoice] << " " << carSep << " ";

        file << car[carChoice].carname << " " << jrnCost<<" ";

        file<<availDriver[randIndex].name<<" "<<availDriver[randIndex].age<<endl;

        file.close();

        int reduceCarquan = del(car[carChoice].carname);
        car.clear();
        car = putInVec();

        delFromAvail(availDriver[randIndex].name);
        

        cout << "Thank you for choosing our platform. Your booking has been confirmed." << endl;
        cout << endl;
        userMenu(name);
    }

    else if (c == 'n')
    {
        
        cout << "Thank You! for checking our platform.....Redirecting you to main menu again." << endl;
        cout << endl;
        userMenu(name);
    }

}

void adminMenu();

void addDriver(){
    cout<<endl;
    cout<<"--------------ADD DRIVER--------------"<<endl<<endl;
    cout<<"---ENTER THE DRIVER DETAILS TO BE ADDED---"<<endl;
    cout<<"\nEnter driver's name : ";
    string dName;
    cin>>dName;

    cout<<"Enter the Age : ";
    int dage;
    cin>>dage;

    ofstream ptr,file;
    ptr.open("drivers.txt",ios::app);
    file.open("availableDrivers.txt",ios::app);
    ptr<<dName<<" "<<dage<<endl;
    file<<dName<<" "<<dage<<endl;
    ptr.close();
    file.close();

    cout<<"\nDriver added successfully!!!"<<endl;

    Driver.clear();
    Driver = dataDriver("drivers.txt");
    availDriver.push_back({dName,dage});
    adminMenu();
}

void delDriver(){
    cout<<endl;
    cout<<"--------------FIRE DRIVER--------------"<<endl<<endl;
    cout<<"Showing Driver's hired: "<<endl<<endl;

    for(int i=0;i<Driver.size();i++){
        cout<<"["<<i+1<<"]"<<" Name: "<<Driver[i].name<<"\n    Age: "<<Driver[i].age;
        cout<<endl<<endl;
    }
    
    int inChoice;
    cout<<"Enter the number of the Driver in the list which you want to Fire: ";
    cin>>inChoice;

    if(inChoice<1 || inChoice>car.size()){
        cout<<"INVALID CHOICE!!!"<<endl;
        cout<<"Sending you back to admin menu..."<<endl;
        adminMenu();
    }
    int j=0;
    for(j=0;j<availDriver.size();j++){
        if(availDriver[j].name==Driver[inChoice-1].name){
            break;
        }
    }
    if(j!=availDriver.size()){

        availDriver.erase(availDriver.begin()+j);
        ofstream ptr;
        ptr.open("availableDrivers.txt");
        for(int i=0;i<availDriver.size();i++){
            ptr<<availDriver[i].name<<" "<<availDriver[i].age<<endl;
        }
        ptr.close();

    }
    Driver.erase(Driver.begin()+inChoice-1);
    
    ofstream file;
    file.open("drivers.txt");

    for(int i=0; i<Driver.size(); i++){

        file<<Driver[i].name<<" "<<Driver[i].age<<endl;

    }

    file.close();
    cout<<"\nSuccessfully fired selected driver!"<<endl;
    adminMenu();
}

void mergeByName(vector<driver>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temporary arrays
    vector<driver> leftArr(n1);
    vector<driver> rightArr(n2);

    // Copy data to temporary arrays
    for (int i = 0; i < n1; i++)
        leftArr[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        rightArr[j] = arr[mid + 1 + j];

    // Merge the temporary arrays back into the main array
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (leftArr[i].name <= rightArr[j].name) {
            arr[k] = leftArr[i];
            i++;
        }
        else {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of leftArr[], if there are any
    while (i < n1) {
        arr[k] = leftArr[i];
        i++;
        k++;
    }

    // Copy the remaining elements of rightArr[], if there are any
    while (j < n2) {
        arr[k] = rightArr[j];
        j++;
        k++;
    }
}

// Merge function for merging two sorted sub-arrays by age
void mergeByAge(vector<driver>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temporary arrays
    vector<driver> leftArr(n1);
    vector<driver> rightArr(n2);

    // Copy data to temporary arrays
    for (int i = 0; i < n1; i++)
        leftArr[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        rightArr[j] = arr[mid + 1 + j];

    // Merge the temporary arrays back into the main array
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (leftArr[i].age <= rightArr[j].age) {
            arr[k] = leftArr[i];
            i++;
        }
        else {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of leftArr[], if there are any
    while (i < n1) {
        arr[k] = leftArr[i];
        i++;
        k++;
    }

    // Copy the remaining elements of rightArr[], if there are any
    while (j < n2) {
        arr[k] = rightArr[j];
        j++;
        k++;
    }
}

// Merge sort function for sorting by name
void mergeSort(vector<driver>& arr, int left, int right,int choice) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        // Sort first and second halves
        if(choice==1){
        mergeSort(arr, left, mid,choice);
        mergeSort(arr,mid+1,right,choice);
        mergeByName(arr,left,mid,right);
        }

        else{
            mergeSort(arr, left, mid,choice);
            mergeSort(arr,mid+1,right,choice);
            mergeByAge(arr,left,mid,right);
        }
    }
    else{
        return;
    }
}

void printDriverList(){

    cout<<endl;
    cout<<"-------------DRIVER LIST-------------"<<endl<<endl;
    cout<<"1.Sorted by Name"<<endl;
    cout<<"2.Sorted by Age"<<endl;
    int choice;
    cout<<"Choice: ";
    cin>>choice;

    switch(choice){
        case 1:
            mergeSort(Driver,0,Driver.size()-1,1);
            break;
        
        case 2:
            mergeSort(Driver,0,Driver.size()-1,2);
            break;
        
        default:
            cout<<"INVALID CHOICE!!!!"<<endl;
            cout<<"CHOOSE AGAIN..."<<endl;
            printDriverList();
    }

    for(int i=0;i<Driver.size();i++){
        cout<<"["<<i+1<<"]"<<" Name: "<<Driver[i].name<<"\n    Age: "<<Driver[i].age;
        cout<<endl<<endl;
    }

    adminMenu();

}

void printCarList(){

    for(int i=0;i<car.size();i++){
        cout<<"["<<i+1<<"]"<<" Name: "<<car[i].carname<<"\n    Qty: "<<car[i].quantity<<"\n    Rent per Km: Rs."<<car[i].charge;
        cout<<endl<<endl;
    }

}

void modCar(){

    cout<<endl;
    cout<<"--------------MODIFY CAR DATA FROM HAUL--------------"<<endl<<endl;
    cout<<"Showing Cars in haul : "<<endl<<endl;

    printCarList();

    int inChoice;
    cout<<"Enter the number of the car in the list which you want to modify: ";
    cin>>inChoice;

    if(inChoice<1 || inChoice>car.size()){
        cout<<"INVALID CHOICE!!!"<<endl;
        cout<<"Sending you back to admin menu..."<<endl;
        adminMenu();
    }

    cout<<"What would you like to modify (1.Quantity 2.Per Km Rent): ";
    int modChoice;
    cin>>modChoice;

    ofstream file;
    file.open("cars.txt");

    switch(modChoice){

        case 1:
            cout<<endl;
            cout<<"New Quantity:";
            int newQuantity;
            cin>>newQuantity;
            car[inChoice-1].quantity = newQuantity;
            for(int i=0; i<car.size(); i++){

                file<<car[i].carname<<" "<<car[i].quantity<<" "<<car[i].charge<<endl;

            }
            break;

        case 2:
            cout<<endl;
            cout<<"New Per Km Rent:";
            int newRent;
            cin>>newRent;
            car[inChoice-1].charge = newRent;
            for(int i=0; i<car.size(); i++){

                file<<car[i].carname<<" "<<car[i].quantity<<" "<<car[i].charge<<endl;

            }
            break;

        default:
            cout<<"INVALID CHOICE!!!"<<endl;
            cout<<"Sending you back to admin menu..."<<endl;
            adminMenu();
            break;
    }

    cout<<"\nSUCCESSFULLY MODIFIED!!!"<<endl;
    file.close();
}

void delCar(){
    cout<<endl;
    cout<<"--------------DELETE CAR FROM HAUL--------------"<<endl<<endl;
    cout<<"Showing Cars in haul : "<<endl<<endl;

    printCarList();
    
    int inChoice;
    cout<<"Enter the number of the car in the list which you want to delete: ";
    cin>>inChoice;

    if(inChoice<1 || inChoice>car.size()){
        cout<<"INVALID CHOICE!!!"<<endl;
        cout<<"Sending you back to admin menu..."<<endl;
        adminMenu();
    }

    car.erase(car.begin()+inChoice-1);
    ofstream file;
    file.open("cars.txt");

    for(int i=0; i<car.size(); i++){

        file<<car[i].carname<<" "<<car[i].quantity<<" "<<car[i].charge<<endl;

    }

    file.close();
    cout<<"\nSuccessfully deleted selected car!"<<endl;
    adminMenu();

}

void addCar(){

    cout<<endl;
    cout<<"--------------ADD CAR TO HAUL--------------"<<endl<<endl;
    cout<<"---ENTER THE CAR DETAILS TO BE ADDED---"<<endl;
    cout<<"\nEnter car name : ";
    string carName;
    cin>>carName;

    cout<<"Enter the number of "<< carName<<" being added to the haul : ";
    int qty;
    cin>>qty;

    cout<<"Enter the Per km cost for renting the car : ";
    int pcost;
    cin>>pcost;

    ofstream ptr;
    ptr.open("cars.txt",ios::app);
    ptr<<carName<<" "<<qty<<" "<<pcost<<endl;

    ptr.close();

    cout<<"\nCar added to haul successfully!!!"<<endl;

    car.clear();
    car = putInVec();
    adminMenu();
    
}

void userMenu(string name)
{

    cout << "--------------USER MENU--------------" << endl
         << endl;
    cout << "1.Book a ride..." << endl;
    cout << "2.Check bookings..." << endl;
    cout << "3.Cancel a ride..." << endl;
    cout << "4.Exit..." << endl;

    cout << "Enter your Choice :";
    int choice;
    cin >> choice;

    switch (choice)
    {
    case 1:
        booking(name);
        break;
    case 2:
        check(name);
        break;
    case 3:
        cancel(name);
        break;
    case 4:
        exit(0);
        break;
    case 5:
        // login();
        // break;
    case 6:
        // register();
        // break;
    case 69:
        registration();
        break;
    default:
        // userMenu();
        break;
    }
}

void adminMenu(){

    cout<<endl;
    cout<<"-------------ADMIN MENU -------------"<<endl<<endl;
    cout<<"1.Add new car to the haul"<<endl;
    cout<<"2.Delete car from the haul"<<endl;
    cout<<"3.Modify car details"<<endl;
    cout<<"4.Check sorted driver's list"<<endl;
    cout<<"5.Add driver"<<endl;
    cout<<"6.Delete driver"<<endl;
    cout<<"7.Exit"<<endl<<endl;

    int choice;
    cout<<"Enter your choice : ";
    cin>>choice;

    switch(choice){

        case 1:
            addCar();
            break;

        case 2:
            delCar();
            break;

        case 3:
            modCar();
            break;

        case 4:
            printDriverList();
            break;

        case 5:
            addDriver();
            break;

        case 6:
            delDriver();
            break;

        case 7:
            exit(0);
            break;

        default:
            cout<<"Invalid choice"<<endl;
            adminMenu();
            break;
    }
}

void enterAdmin(){

    cout<<endl;
    cout<<"-------------ADMIN LOGIN-------------"<<endl<<endl;
    string adminName;
    string pass;

    ifstream file;
    file.open("Admin.txt");

    string line;

    getline(file,line);
    stringstream ss(line);
    string checkUser;
    string checkPass;
    ss>>checkUser>>checkPass;

    cout<<"Enter admin name: ";
    cin>>adminName;

    cout<<"Enter admin password: ";
    cin>>pass;

    if(adminName!=checkUser || pass!=checkPass){
        cout<<"Incorrect ID and password..."<<endl;
        cout<<"Login Again..."<<endl;
        enterAdmin();
    }

    else{
        cout<<"SUCCESSFULLY LOGGED IN"<<endl;
        adminMenu();
    }

    file.close();
}

void details()
{

    cout << endl;
    cout << "------------WELCOME ABOARD USER------------" << endl
         << endl;
    cout << "1.Register yourself" << endl;
    cout << "2.Login to your ID" << endl
         << endl;

    int choice;
    cout << "Shoot your shot : ";
    cin >> choice;

    switch (choice)
    {

    case 1:
        registration();
        break;

    case 2:
        login();
        break;

    default:
        cout << "It seems you mistyped!!!!" << endl
             << endl;
        details();
    }
}

void menu()
{
    cout << "------------ROAD TRAVEL AGENCY------------" << endl
         << endl;
    cout << "1.User space" << endl;
    cout << "2.Admin space" << endl;
    cout << "3.Exit" << endl;
    cout << "Enter your Choice :";
    int choice;
    cin >> choice;

    switch (choice)
    {
    case 1:
        details();
        break;
    case 2:
        enterAdmin();
        break;
    case 3:
        exit(0);
        break;

    default:
        menu();
        break;
    }
}

int main()
{
    menu();
    return 0;
}