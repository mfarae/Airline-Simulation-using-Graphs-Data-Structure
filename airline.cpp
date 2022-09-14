/****************************************************
Names(REG): Muhammad Farae(2020292)
Course/Section: CS221/B
Date: 9/January/2021
DSA PROJECT OPTIONAL TASK-AIRLINE MANAGEMENT SYSTEM
******************************************************/
/****************************************************
 * This code implements the graph algorithms to implement airline management system
 * It implements graphs, sorting both flights by arrival and departure, breadth first search and display of the flight information in a tabular form
 * **************************************************/

#include <iostream>
#include <string>
using namespace std;

struct node;
struct edge; // struct prototyping
struct node
{
    string name;
    node *next; // adjency list node implementation, which are vertices
    edge *EHead;
    edge *ETail;
}*head, *tail; // global declarations

int graph_size = 0;

struct edge // edge definition
{
    string adj_node;
    float cost;
    edge* next_edge;
};

void append(string name) // code ton define vertex
{
    if (head == nullptr)
    {
        head = new node;
        head->name = name;
        head->EHead = new edge;
        head->EHead->next_edge = nullptr;
        head->ETail = head->EHead;
        head->next = nullptr;
        tail = new node;
        tail->next = head;
        tail = head;
        graph_size++;
        return;
    }
    
    node *ptr = new node;
    ptr->name = name;
    ptr->EHead = new edge;
    ptr->EHead->next_edge = nullptr;
    ptr->ETail = ptr->EHead;
    ptr->next = nullptr;
    tail->next = ptr;
    tail = ptr;
    graph_size++;
}

edge* add_edge(string adj_node, float cost, edge* ETail) // code to define edge
{
    edge *E = new edge;
    E->cost = cost;
    E->adj_node = adj_node;
    E->next_edge = nullptr;
    ETail->next_edge = E;
    ETail = E;
    return ETail;
}


void display_graph() // code to iteratively traverse the adjacency list and display the entire the graph
{
    node *ptr = head;
    while (ptr != nullptr)
    {
        cout << ptr->name << " connects with: ";
        edge* e = ptr->EHead->next_edge;
        while (e != nullptr)
        {
            cout << e->adj_node << "(" << e->cost <<"minutes), ";
            e = e->next_edge;
        }
        ptr = ptr->next;
        cout << endl;
    }
}

void sort_flight_departures(string name)
{
    node *ptr = head;
    while (ptr->name != name)
    {
        ptr = ptr->next;
    }
    edge *e = ptr->EHead->next_edge;
    edge *array = new edge[graph_size]; // copy all the edges to sort into this dynamic array
    int index = 0;
    while(true)
    {
        if (e->next_edge == nullptr)
        {
            array[index].adj_node = e->adj_node;
            array[index].cost = e->cost;
            index++;
            break;
        }
        array[index].adj_node = e->adj_node;
        array[index].cost = e->cost;
        index++;
        e = e->next_edge;
    }
    for (int i = 0; i < index; i++) // sorting is done through bubblesort
    {
        for (int j = 1; j < index - i; j++)
        {
            if (array[j].cost < array[j-1].cost)
            {
                edge temp = array[j];
                array[j] = array[j-1];
                array[j-1] = temp;
            }
        }
    }
    cout << "Flights departing from: " << name << endl;
    for (int i = 0; i < index; i++)
    {
        cout << "Destination: " << array[i].adj_node << ", time: " << array[i].cost << " minutes" << endl;
    }
    delete []array;
}

void sort_flight_arrivals(string name) // same logic as sorting by flight departures except the sorting is done on cities that are being arrived at
{
    node *ptr = head;
    edge *arrivals = new edge[graph_size];
    int index = 0;
    while (ptr != nullptr)
    {
        edge *e = ptr->EHead->next_edge;
        while(e != nullptr)
        {
            if (e->adj_node == name)
            {
                arrivals[index].adj_node = ptr->name;
                arrivals[index].cost = e->cost;
                index++;
            }
            e = e->next_edge;
        }
        ptr = ptr->next;
    }
    for (int i = 0; i < index; i++)
    {
        for (int j = 1; j < index - i; j++)
        {
            if (arrivals[j].cost < arrivals[j-1].cost)
            {
                edge temp = arrivals[j];
                arrivals[j] = arrivals[j-1];
                arrivals[j-1] = temp;
            }
        }
    }
    cout << "Flights arriving at: " << name << endl;
    for (int i = 0; i < index; i++)
    {
        cout << "Source: " << arrivals[i].adj_node << ", time: " << arrivals[i].cost << " minutes"  << endl;
    }
    delete []arrivals;
}

//----------------------------------
struct queue // definition of queue using dynamic array
{
    string *q;
    int size, front, rear;
    queue(int size)
    {
        q = new string[size];
        front = rear = -1;
    }
    void enqueue(string name)
    {
        q[++rear] = name;
        front = 0;
    }
    string dequeue()
    {
        string de_val = q[front];
        for (int i = 0; i < rear; i++)
        {
            q[i] = q[i+1];
        }
        rear--;
        return de_val;
    }
    void display()
    {
        for (int i = 0; i <= rear; i++)
        {
            cout << q[i] << " ";
        }
        cout << endl;
    }
};

int node_number(string name) // convert the names of nodes to their equivalent node positions for BFS
{
    if (name == "Islamabad")
        return 1;
    if (name == "Rawalpindi")
        return 2;
    if (name == "Karachi")
        return 3;
    if (name == "Lahore")
        return 4;
    if (name == "Peshawar")
        return 5;
}

void bfs(string starting_name)
{
    queue q(graph_size);
    int *status = new int[graph_size]; //0 means unvisited, 1 means unprocessed, 2 means processed
    for (int i = 0; i < graph_size; i++) // intializes the status array to zero
    {
        status[i] = 0;
    }
    node *ptr = head;
    bool all_processed = 0;
    string dequeued = starting_name;
    int deq = node_number(dequeued);
    status[deq-1] = 2; // update the starting node to be processed
    while (all_processed == false)
    {
        ptr = head;
        all_processed = true;
        while (ptr->name != dequeued)
        {
            ptr = ptr->next; // traverse the node until the vertex that was dequeued
        }
        edge *e = ptr->EHead->next_edge; // declare a traversal pointer to enqueue all nodes it connects with
        while (e != nullptr)
        {
            if (status[node_number(e->adj_node)-1] != 0) // if the node has already been processed or visited before, skip enqueing
            {
                e = e->next_edge;
                continue;
            }
            q.enqueue(e->adj_node);  // otherwise enqueue
            status[node_number(e->adj_node)-1] = 1;
            e = e->next_edge; // traverse next until nullptr
        }
        dequeued = q.dequeue(); // dequeue the first element
        cout << "\n" << dequeued << " can be visited." << endl;
        status[node_number(dequeued)-1] = 2; // update status to processed

        for (int i = 0; i < graph_size; i++) // check if all nodes have been processed
        {
            if (status[i] != 2)
            {
                all_processed = false; //deallocate memory from heap to avoid memory leak
            }
        }
    }
    delete []status;
}
//----------------------------------------------------------------------------------

int main()
{
    // cost is in minutes
    append("Islamabad");
    append("Rawalpindi");
    append("Karachi");
    append("Lahore");
    append("Peshawar");
    cout << "\n\n";
    //establishing connections
    node* ptr = head;
    cout << ptr->name << endl;
    ptr->ETail = add_edge("Peshawar", 120, ptr->ETail); //ptr at Islamabad
    ptr->ETail = add_edge("Lahore", 240, ptr->ETail);
    ptr->ETail = add_edge("Karachi", 270, ptr->ETail);

    ptr = ptr->next; // ptr at Rawalpindi
    cout << ptr->name << endl;
    ptr->ETail = add_edge("Peshawar", 350, ptr->ETail);
    ptr->ETail = add_edge("Lahore", 250, ptr->ETail);
    ptr->ETail = add_edge("Karachi", 100, ptr->ETail);

    ptr = ptr->next; // ptr at Karachi
    cout << ptr->name << endl;
    ptr->ETail = add_edge("Islamabad", 400, ptr->ETail);
    ptr->ETail = add_edge("Lahore", 300, ptr->ETail);

    ptr = ptr->next;//ptr at Lahore
    cout << ptr->name << endl;
    ptr->ETail = add_edge("Karachi", 350, ptr->ETail);

    ptr = ptr->next; // ptr at Peshawar
    cout << ptr->name << endl;
    ptr->ETail = add_edge("Islamabad", 250, ptr->ETail);


    //Show a list of all the cities serviced
    display_graph();
    cout << "\n\n";
    //Show a list of flight departures for a city, sorted by the time of departure. (10)
    sort_flight_departures("Rawalpindi");
    cout << "\n\n";
    // Show a list of flight arrivals for a city, sorted by the time of arrival. (10)
    sort_flight_arrivals("Karachi");
    cout << "\n\n";
    // Show a list of all the cities which can be reached from a particular city. (10)
    cout << "\n\nAll the cities that can be travelled to from Rawalpindi printed using breadth first search";
    bfs("Rawalpindi");
    // Show the list of cities in the shortest path between any two cities. (20)
    
     // Find the shortest route between any two cities. (20)
    return 0;
}