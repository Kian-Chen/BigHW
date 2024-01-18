/* 2250951 ¹âµç ³Â÷è°² */
#pragma once
#include"cmd_console_tools.h"
int menu(void (*draw)(), int max, int is_ab = false);
void drawBlockFromCenterPixCoord(struct node center_pix, int bg = COLOR_BLACK, int fg = COLOR_HWHITE);
void eliminateBlockFromCenterPixCoord(struct node center_pix, int bg = COLOR_HWHITE, int fg = COLOR_BLACK);

struct node
{
	int x;
	int y;
};

template <typename T>
class List
{
private:
    struct Node
    {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* head;
    size_t count;

public:
    List() : head(nullptr), count(0) {}

    ~List()
    {
        while (head != nullptr)
        {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void push_back(const T& value)
    {
        Node* newNode = new Node(value);
        if (head == nullptr)
        {
            head = newNode;
        }
        else
        {
            Node* temp = head;
            while (temp->next != nullptr)
            {
                temp = temp->next;
            }
            temp->next = newNode;
        }
        count++;
    }

    size_t size() const
    {
        return count;
    }

    void resize(size_t newSize)
    {
        while (count < newSize)
        {
            push_back(T());
        }
        while (count > newSize)
        {
            pop_back();
        }
    }

    T& operator[](size_t index)
    {
        Node* temp = head;
        for (size_t i = 0; i < index; i++)
        {
            temp = temp->next;
        }

        return temp->data;
    }

    const T& operator[](size_t index) const
    {

        Node* temp = head;
        for (size_t i = 0; i < index; i++)
        {
            temp = temp->next;
        }

        return temp->data;
    }

private:
    void pop_back()
    {
        if (head == nullptr)
        {
            return;
        }

        if (count == 1)
        {
            delete head;
            head = nullptr;
        }
        else
        {
            Node* temp = head;
            while (temp->next->next != nullptr)
            {
                temp = temp->next;
            }
            delete temp->next;
            temp->next = nullptr;
        }
        count--;
    }
};

struct node operator+(struct node p1,struct node p2);
struct node operator-(struct node p1, struct node p2);