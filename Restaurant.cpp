#include "main.h"

extern int MAXSIZE;

class imp_res : public Restaurant
{
private:
    customer *customerListHead;
    customer *waiting_customer_list;
    customer *initialList;
    customer *pos_X;
    customer *reverse_customer_list;
    int size = 0, size2 = 0, size3 = 0, size4 = 0;
    int number_of_line = 1;
    int N = 0;
    int max_res = 0;
    bool isCheckName = false;

public:
    imp_res() : customerListHead(nullptr), waiting_customer_list(nullptr), initialList(nullptr), pos_X(nullptr), reverse_customer_list(nullptr)
    {
    }

    void write_file()
    {
    }

    void display(customer *head, int size)
    {
        // cout << size << endl;
        // cout << "Point X: " << pos_X->name << endl;
        if (!head)
        {
            // cout << "Customer list is empty." << endl;
            return;
        }
        // cout << "Customers in the list:" << endl;
        customer *current = head;
        do
        {
            current->print();
            current = current->next;
        } while (current != head);
    }

    void add_before(customer *current, string name, int energy, customer *&head)
    {
        customer *cus = new customer(name, energy, current->prev, current);
        current->prev->next = cus;
        current->prev = cus;
        if (current == head)
        {
            head = cus;
        }
    }

    void add_after(customer *current, string name, int energy)
    {
        customer *cus = new customer(name, energy, current, current->next);
        current->next->prev = cus;
        current->next = cus;
    }

    bool checkName(string name, int energy)
    {
        customer *tmp = initialList;
        bool check1 = false, check2 = false;
        if (tmp)
        {
            do
            {
                if (tmp->name == name)
                {
                    check1 = true;
                    break;
                    return true;
                }
                tmp = tmp->next;
            } while (tmp != initialList);
        }
        tmp = waiting_customer_list;
        if (tmp)
        {
            do
            {
                if (tmp->name == name)
                {
                    check2 = true;
                    break;
                    return true;
                }
                tmp = tmp->next;
            } while (tmp != waiting_customer_list);
        }
        if (isCheckName)
        {
            return false;
        }
        if (check1 == true) //&& check2 == false)
        {
            return true;
        }
        return false;
        // if (tmp->name == name)
        // {
        // 	return true;
        // }
        // else
        // {
        // 	while (tmp->next != customerListHead)
        // 	{
        // 		tmp = tmp->next;
        // 		if (name == tmp->name)
        // 		{
        // 			delete[] tmp;
        // 			return true;
        // 		}
        // 	}
        // }
    }

    void updateX(customer *head, string name, int energy)
    {
        customer *tmp = head;
        while (tmp->name != name)
        {
            tmp = tmp->next;
        }
        pos_X = tmp;
    }

    void add_customer(customer *&head, string name, int energy)
    {

        if (!head)
        {
            // The list is empty, make the new customer circular
            customer *cus = new customer(name, energy, nullptr, nullptr);
            cus->prev = cus;
            cus->next = cus;
            head = cus;
        }
        else
        {
            if (size < MAXSIZE / 2)
            {
                customer *current = head;

                while (current->name != pos_X->name)
                {
                    current = current->next;
                }
                if (current->energy <= energy)
                {
                    add_after(current, name, energy);
                }
                else
                {
                    add_before(current, name, energy, head);
                }
            }
            else
            {
                customer *tmp = head;
                int i = 0;
                while (tmp->name != pos_X->name)
                {
                    tmp = tmp->next;
                    i++;
                }
                int max_res = 0;
                int real_val_res = 0;
                int index = 0;
                do
                {
                    int value = energy - tmp->energy;
                    int value1 = (value < 0 ? -value : value);
                    if (value1 > max_res)
                    {
                        max_res = value1;
                        index = i;
                        real_val_res = value;
                    }
                    i++;
                    if (i >= size)
                        i = 0;
                    tmp = tmp->next;
                } while (tmp->name != pos_X->name);
                tmp = head;
                for (int i = 0; i < index; i++)
                {
                    tmp = tmp->next;
                }
                if (real_val_res < 0)
                {
                    add_before(tmp, name, energy, head);
                }
                else
                {
                    add_after(tmp, name, energy);
                }
            }
        }
    }

    void RED(string name, int energy)
    {
        number_of_line++;
        if (energy != 0)
        {
            if (!customerListHead)
            {
                size++;
                add_customer(customerListHead, name, energy);
                updateX(customerListHead, name, energy);
                if (!isCheckName)
                {
                    add_customer(initialList, name, energy);
                }
                // add_customer(pos_X, name, energy);
                pos_X = customerListHead;
                updateX(customerListHead, name, energy);
                isCheckName = false;
            }
            else
            {
                customer *tmp = initialList;
                if (!checkName(name, energy))
                {
                    if (!isCheckName && size3 < MAXSIZE * 2)
                    {
                        tmp = tmp->prev;
                        add_after(tmp, name, energy);
                    }
                    isCheckName = false;
                    if (size < MAXSIZE)
                    {
                        add_customer(customerListHead, name, energy);
                        updateX(customerListHead, name, energy);
                        size++;
                    }
                    else
                    {
                        if (size2 < MAXSIZE)
                        {
                            size2++;
                            if (!waiting_customer_list)
                            {
                                customer *cus = new customer(name, energy, nullptr, nullptr);
                                cus->prev = cus;
                                cus->next = cus;
                                waiting_customer_list = cus;
                            }
                            else
                            {
                                customer *tmp = waiting_customer_list;
                                while (tmp->next != waiting_customer_list)
                                {
                                    tmp = tmp->next;
                                }
                                add_after(tmp, name, energy);
                            }
                        }
                    }
                }
            }
            size3 = size + size2;
        }
    }

    void delete_customer(customer *&head, int position, int &size)
    {
        if (size - 1 == 0)
        {
            delete_all(head, size);
            return;
        }
        if (position == 1 || position == 0)
        {
            head = head->next;
            head->prev = head->prev->prev;
            head->prev->next = head;
        }
        else if (1 < position && position < size - 1)
        {
            head->prev->next = head->next;
            head->next->prev = head->prev;
            head = head->next;
        }
        else
        {
            head->prev->next = head->next;
            head->next->prev = head->prev;
            head = head->next;
        }
        size--;
    }

    void delete_all(customer *&head, int &size)
    {
        if (head)
        {
            head->next = nullptr;
            head->prev = nullptr;
            head = nullptr;
            size = 0;
        }
    }

    void BLUE(int num)
    {
        number_of_line++;
        if (!customerListHead)
        {
            return;
        }
        else
        {
            if (num >= size)
            {
                int count = size;
                while (count--)
                {
                    customer *tmp = initialList;
                    delete_customer(tmp, 1, size3);
                    initialList = tmp;
                }
                delete_all(customerListHead, size);
                delete_all(pos_X, size);
                delete_all(reverse_customer_list, size4);
            }
            else
            {
                while (num--)
                {
                    int index = 0;
                    customer *tmp = initialList;
                    customer *tmp2 = customerListHead;
                    while (tmp2->name != tmp->name)
                    {
                        tmp2 = tmp2->next;
                        index++;
                    }
                    if (tmp2->energy > 0)
                    {
                        delete_customer(tmp2, index, size);
                        updateX(tmp2, tmp2->name, tmp2->energy);
                    }
                    else
                    {
                        delete_customer(tmp2, index, size);
                        updateX(tmp2, tmp2->prev->name, tmp2->prev->energy);
                    }
                    delete_customer(initialList, 1, size3);
                    customerListHead = tmp2;
                }
            }
        }
        while (size < MAXSIZE && waiting_customer_list != nullptr)
        {
            isCheckName = true;
            string name1 = waiting_customer_list->name;
            int energy1 = waiting_customer_list->energy;
            delete_customer(waiting_customer_list, 1, size2);
            RED(name1, energy1);
            number_of_line--;
        }
        // display(waiting_customer_list, size2);
    }

    int setValue(customer *head, int position)
    {
        while (position--)
        {
            head = head->next;
        }
        int val = head->energy;
        return val;
    }

    void setValue(customer *head, int position, string &name, int &val)
    {
        while (position--)
        {
            head = head->next;
        }
        val = head->energy;
        name = head->name;
    }

    void updateValue(customer *&head, int position, string name, int energy)
    {
        while (position--)
        {
            head = head->next;
        }
        head->name = name;
        head->energy = energy;
    }

    void swap(customer *&head, int pos1, int pos2)
    {

        string name1, name2;
        int ener1, ener2;
        setValue(head, pos1, name1, ener1);
        setValue(head, pos2, name2, ener2);
        customer *tmp = head;
        // logging << "swapping " << name1 << " " << ener1 << " and " << name2 << " " << ener2 << endl;
        // cout << "swapping " << name1 << " " << ener1 << " and " << name2 << " " << ener2 << endl;
        updateValue(tmp, pos1, name2, ener2);
        tmp = head;
        updateValue(tmp, pos2, name1, ener1);
        N++;
    }

    void inssort2(customer *&head, int n, int incr)
    {
        for (int i = incr; i < n; i += incr)
        {
            int j = i;
            for (j = i; j >= incr && abs(setValue(head, j)) > abs(setValue(head, j - incr)); j -= incr)
            {
                swap(head, j, j - incr);
            }
        }
    }

    void shellsort(customer *&head, int n)
    {
        for (int i = n / 2; i > 2; i /= 2)
        {
            for (int j = 0; j < i; j++)
            {
                customer *temp = head;
                for (int z = 0; z < j; z++)
                {
                    temp = temp->next;
                }
                inssort2(temp, n - j, i);
            }
        }
        inssort2(head, n, 1);
    }
    void PURPLE()
    {
        N = 0;
        number_of_line++;
        if (waiting_customer_list)
        {
            customer *tmp = waiting_customer_list->next;
            int n = 0;
            int i = 1;
            int max = abs(waiting_customer_list->energy);
            while (tmp != waiting_customer_list)
            {
                int value = abs(tmp->energy);
                if (value >= max)
                {
                    max = value;
                    n = i;
                }
                i++;
                tmp = tmp->next;
            }
            shellsort(waiting_customer_list, n + 1);
            // cout << N << endl;
            BLUE(N % MAXSIZE);
            number_of_line--;
            // display(waiting_customer_list, size2);
        }
    }

    void add_customer2(customer *&head, string name, int energy)
    {
        if (!head)
        {
            // The list is empty, make the new customer circular
            customer *cus = new customer(name, energy, nullptr, nullptr);
            cus->prev = cus;
            cus->next = cus;
            head = cus;
        }
        else
        {
            customer *tmp = head;
            while (tmp->next != head)
            {
                tmp = tmp->next;
            }
            add_before(tmp, name, energy, head);
        }
    }

    void create_reversal_list(string name, int energy)
    {
        size4++;
        if (!reverse_customer_list)
        {
            add_customer(reverse_customer_list, name, energy);
        }
        else
        {
            customer *tmp = reverse_customer_list;
            while (tmp->next != reverse_customer_list)
            {
                tmp = tmp->next;
            }
            add_after(tmp, name, energy);
        }
        // add_customer2(reverse_customer_list, name, energy);
    }

    // void changeValue(customer *&head, string name, int energy)
    // {
    // 	head->name = name;
    // 	head->energy = energy;
    // }

    // void REVERSAL2() // fake REVERSAL
    // {
    // 	number_of_line++;
    // 	string pivot_name = pos_X->name;
    // 	int index = 1;
    // 	customer *tmp = customerListHead;
    // 	while (tmp->name != pos_X->name)
    // 	{
    // 		tmp = tmp->next;
    // 		index++;
    // 	}
    // 	customer *positive = tmp->next;
    // 	customer *negative = tmp->next;
    // 	customer *temp = pos_X;
    // 	int length = size;
    // 	while (length--)
    // 	{
    // 		if (temp->energy < 0)
    // 		{
    // 			while (negative->energy > 0)
    // 			{
    // 				negative = negative->next;
    // 			}
    // 			changeValue(temp, negative->name, negative->energy);
    // 		}
    // 		else
    // 		{
    // 			while (positive->energy < 0)
    // 			{
    // 				positive = positive->next;
    // 			}
    // 			changeValue(temp, positive->name, positive->energy);
    // 		}
    // 	}
    // 	length = size;
    // 	delete_all(customerListHead, length);
    // 	customerListHead = pos_X;
    // 	while (customerListHead->name != pivot_name)
    // 	{
    // 		customerListHead = customerListHead->next;
    // 	}
    // }

    void REVERSAL() // real REVERSAL
    {
        number_of_line++;
        int index = 1;
        customer *tmp = customerListHead;
        if (!tmp)
        {
            return;
        }
        while (tmp->name != pos_X->name)
        {
            tmp = tmp->next;
            index++;
        }
        customer *positive = pos_X->next;
        customer *negative = pos_X->next;
        customer *temp = pos_X;
        int length = size;
        while (length--)
        {
            if (temp->energy < 0)
            {
                while (negative->energy > 0)
                {
                    negative = negative->next;
                }
                create_reversal_list(negative->name, negative->energy);
                negative = negative->next;
            }
            if (temp->energy > 0)
            {
                while (positive->energy < 0)
                {
                    positive = positive->next;
                }
                create_reversal_list(positive->name, positive->energy);
                positive = positive->next;
            }
            temp = temp->prev;
        }
        length = size;
        delete_all(customerListHead, size);
        customer *current = reverse_customer_list;
        while (current->name != pos_X->name)
        {
            current = current->next;
        }
        do
        {
            size++;
            add_customer2(customerListHead, current->name, current->energy);
            current = current->prev;
        } while (current->name != pos_X->name);
        // customerListHead = reverse_customer_list;
        while (customerListHead->name != pos_X->name)
        {
            customerListHead = customerListHead->next;
        }
        length = size;
        delete_all(pos_X, length);
        pos_X = customerListHead;
        delete_all(reverse_customer_list, size4);
        // while (customerListHead->name != current->name)
        // {
        // 	customerListHead = customerListHead->next;
        // }
        // pos_X = customerListHead;
        // display(customerListHead, size4);
        // cout << "reversal" << endl;
    }

    void print_unlimit_void(customer *a, customer *b)
    {
        a = a->prev;
        customer *tmp = a;
        do
        {
            customer *tmp2 = b;
            do
            {
                if (tmp->name == tmp2->name)
                {
                    tmp->print();
                    break;
                }
                tmp2 = tmp2->next;
            } while (tmp2 != b);
            tmp = tmp->prev;
        } while (tmp != a);
    }

    void UNLIMITED_VOID()
    {
        int length_max = 0;
        customer *tmp = pos_X;
        if (!tmp)
        {
            return;
        }
        int start = 0, end = 0;
        int start1 = 0, end1 = 0;
        start1 = start;
        end1 = end;
        number_of_line++;
        int min = INT_MAX;
        int value = pos_X->energy;
        int sum = pos_X->energy;
        cout << "UNLIMITED_VOID line " << number_of_line << endl;
        customer *curr2 = nullptr;
        customer *curr = pos_X;
        for (int i = 0; i < size; i++)
        {
            start1 = i, end1 = i;
            customer *temp = tmp;
            // value = temp->energy;
            sum = temp->energy;
            int count = 1;
            for (int j = 1; j < size; j++)
            {
                end1++;
                temp = temp->next;
                if (end1 >= size)
                {
                    end1 = 0;
                }
                sum += temp->energy;
                count++;
                if (count >= 4 && min >= sum)
                {
                    if ((min == sum && count >= length_max) || (min > sum))
                    {
                        start = start1;
                        end = end1;
                        min = sum;
                        length_max = count;
                    }
                    // if (sum < value)
                    // {
                    //     value = sum;
                    //     if (value < min)
                    //     {
                    //         start = start1;
                    //         end = end1;
                    //         min = value;
                    //     }
                    // }
                }
            }
            tmp = tmp->next;
        }
        int val_check = (start > end) ? size - start + 1 + end : abs(start - end) + 1;
        if (val_check >= 4)
        {
            tmp = pos_X;
            if (start > end)
            {
                for (int i = 0; i < size - start; i++)
                {
                    tmp = tmp->prev;
                }
                for (int i = 0; i < size - start; i++)
                {
                    if (!curr2)
                    {
                        add_customer(curr2, tmp->name, tmp->energy);
                    }
                    else
                    {
                        customer *temp = curr2;
                        temp = temp->prev;
                        add_after(temp, tmp->name, tmp->energy);
                    }
                    tmp = tmp->next;
                }
                tmp = pos_X;
                for (int i = 0; i <= end; i++)
                {
                    customer *temp = curr2;
                    temp = temp->prev;
                    add_after(temp, tmp->name, tmp->energy);
                    tmp = tmp->next;
                }
            }
            else
            {
                for (int i = 0; i < start; i++)
                {
                    tmp = tmp->next;
                }
                for (int i = start; i <= end; i++)
                {
                    if (!curr2)
                    {
                        add_customer(curr2, tmp->name, tmp->energy);
                    }
                    else
                    {
                        customer *temp = curr2;
                        temp = temp->prev;
                        add_after(temp, tmp->name, tmp->energy);
                    }
                    tmp = tmp->next;
                }
            }
            customer *temp1 = curr2;
            customer *temp2 = curr2;
            int min_pivot = curr2->energy;
            do
            {
                temp1 = temp1->next;
                if (temp1->energy < min_pivot)
                {
                    temp2 = temp1;
                    min_pivot = temp1->energy;
                }
            } while (temp1 != curr2);
            curr2 = temp2;
            do
            {
                temp2->print();
                temp2 = temp2->next;
            } while (temp2 != curr2);
            // if (start > end)
            // {
            //     curr = customerListHead;
            //     for (int i = 0; i < size - start + 1; i++)
            //     {
            //         curr = curr->prev;
            //         add_customer2(temp, curr->name, curr->energy);
            //     }
            //     curr = customerListHead;
            //     for (int i = 0; i < end; i++)
            //     {
            //         add_customer2(temp, curr->name, curr->energy);
            //     }
            //     print_unlimit_void(initialList, temp);
            // }
            // else
            // {
            //     curr = customerListHead;
            //     for (int i = 0; i < start; i++)
            //     {
            //         curr = curr->next;
            //     }
            //     int length = abs(start - end) + 1;
            //     for (int i = 0; i < length; i++)
            //     {
            //         curr = curr->next;
            //         add_customer2(temp, curr->name, curr->energy);
            //     }
            //     print_unlimit_void(initialList, temp);
            // }
        }
    }

    void domain_check_delete(customer *&list1, int index)
    {
        bool check = true;
        int j = 0;
        customer *temp = customerListHead;
        customer *tmp = list1;
        delete_customer(list1, index, size3); // list2
        while (j < size)
        {
            if (temp->name == tmp->name)
            {
                if (temp->energy > 0)
                {
                    delete_customer(temp, j, size);
                    updateX(temp, temp->name, temp->energy);
                }
                else
                {
                    delete_customer(temp, j, size);
                    updateX(temp, temp->prev->name, temp->prev->energy);
                }
                customerListHead = temp;
                check = false;
                break;
            }
            else
            {
                j++;
                temp = temp->next;
            }
        }
        if (check)
        {

            if (waiting_customer_list->name == tmp->name)
            {
                delete_customer(waiting_customer_list, j, size2);
            }
            else
            {
                temp = waiting_customer_list;
                j = 0;
                while (j < size2)
                {
                    if (temp->name == tmp->name)
                    {
                        delete_customer(temp, j, size2);
                        break;
                    }
                    else
                    {
                        j++;
                        temp = temp->next;
                    }
                }
            }
        }
    }

    void DOMAIN_EXPANSION()
    {
        number_of_line++;
        cout << "DOMAIN_EXPANSION line " << number_of_line << endl;
        if (initialList)
        {
            customer *tmp = initialList;
            int sum1 = 0, sum2 = 0;
            do
            {
                if (tmp->energy < 0)
                {
                    sum2 += -tmp->energy;
                }
                else
                {
                    sum1 += tmp->energy;
                }
                tmp = tmp->next;
            } while (tmp != initialList);
            int i = size3;
            initialList = initialList->prev;
            if (sum1 >= sum2)
            {
                do
                {
                    i--;
                    if (initialList->energy > 0)
                    {
                        initialList = initialList->prev;
                    }
                    else
                    {
                        initialList->print();
                        domain_check_delete(initialList, i);
                        initialList = initialList->prev;
                    }
                } while (i > 0);
                initialList = initialList->next;
            }
            else
            {
                do
                {
                    i--;
                    if (initialList->energy < 0)
                    {
                        initialList = initialList->prev;
                    }
                    else
                    {
                        initialList->print();
                        domain_check_delete(initialList, i);
                        initialList = initialList->prev;
                    }
                } while (i > 0);
                initialList = initialList->next;
            }
            while (size < MAXSIZE && waiting_customer_list != nullptr)
            {
                isCheckName = true;
                string name1 = waiting_customer_list->name;
                int energy1 = waiting_customer_list->energy;
                delete_customer(waiting_customer_list, 1, size2);
                RED(name1, energy1);
                number_of_line--;
            }
        }
    }

    void LIGHT(int num)
    {
        number_of_line++;
        cout << "LIGHT " << num << " "
             << "line " << number_of_line << endl;
        customer *tmp = pos_X;
        if (!tmp)
        {
            return;
        }
        else
        {
            if (num != 0)
            {
                do
                {
                    tmp->print();
                    if (num < 0)
                        tmp = tmp->prev;
                    else
                        tmp = tmp->next;
                } while (tmp != pos_X);
            }
            else
            {
                tmp = waiting_customer_list;
                display(tmp, size2);
            }
        }
    }
};