#include <bits/stdc++.h>
#include <iostream>

using namespace std;

class Exception : public exception
{
protected:
    //сообщение об ошибке
    char *str;

public:
    Exception(const char *s)
    {
        str = new char[strlen(s) + 1];
        strcpy(str, s);
    }
    Exception(const Exception &e)
    {
        str = new char[strlen(e.str) + 1];
        strcpy(str, e.str);
    }
    ~Exception()
    {
        delete[] str;
    }
    //функцию вывода можно будет переопределить в производных классах, когда будет ясна конкретика
    virtual void print()
    {
        cout << "Exception: " << str << "; " << what();
    }
};

class WrongSizeException : public Exception
{
protected:
    int height, width;

public:
    WrongSizeException(int H, int W) : Exception("Wrong size")
    {
        height = H, width = W;
    }

    WrongSizeException() : Exception("Wrong size")
    {
        height = 0, width = 0;
    }

    WrongSizeException(const WrongSizeException &e) : Exception("Wrong size")
    {
        str = new char[strlen(e.str) + 1];
        strcpy(str, e.str);
        height = e.height, width = e.width;
    }

    virtual void print()
    {
        cout << "Exception: " << str << "; dimensions of matrix: " << height << ", " << width << "; " << what();
    }
};

class IndexOutOfBounds : public Exception
{
private:
    int index1;
    int index2;

public:
    IndexOutOfBounds() : Exception("Wrong index")
    {
        index1 = -1;
        index2 = -1;
    }

    IndexOutOfBounds(int i, int j) : Exception("Wrong index")
    {
        index1 = i;
        index2 = j;
    }
    IndexOutOfBounds(const IndexOutOfBounds &e) : Exception("Wrong size")
    {
        str = new char[strlen(e.str) + 1];
        strcpy(str, e.str);
        index1 = e.index1;
        index2 = e.index2;
    }

    virtual void print()
    {
        cout << "Exception: " << str << "; index_1: " << index1 << "; "
             << "; index_2: " << index2 << "; " << what();
    }
};

class BaseMatrix
{
protected:
    double **ptr;
    int height;
    int width;
    double ScalarProduct(double *arr1, double *arr2, int len)
    {
        double res = 0;
        for (int i = 0; i < height; i++)
            res += arr1[i] * arr2[i];
        return res;
    }

public:
    BaseMatrix(int Height = 2, int Width = 2)
    {
        if (Height < 1 && Width < 1)
        {
            throw WrongSizeException(Height, Width);
        }
        height = Height;
        width = Width;
        ptr = new double *[height];
        for (int i = 0; i < height; i++)
            ptr[i] = new double[width];
    }

    BaseMatrix(const BaseMatrix &M)
    {
        height = M.height;
        width = M.width;
        ptr = new double *[height];
        for (int i = 0; i < height; i++)
            ptr[i] = new double[width];
        for (int i = 0; i < height; i++)
            for (int j = 0; j < height; j++)
                ptr[i][j] = M.ptr[i][j];
    }

    ~BaseMatrix()
    {
        if (ptr != NULL)
        {
            for (int i = 0; i < height; i++)
                delete[] ptr[i];
            delete[] ptr;
            ptr = NULL;
        }
    }

    void print()
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
                cout << ptr[i][j] << " ";
            cout << "\n";
        }
    }

    double &operator()(int row, int column)
    {
        if (row > height || row < 0 || column > width || column < 0)
            throw IndexOutOfBounds(row, column);
        return ptr[row][column];
    }

    BaseMatrix Transponse()
    {
        BaseMatrix Res(width, height);
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
                Res.ptr[j][i] = ptr[i][j]; // Res(j,i) = ptr[i][j];

        return Res;
    }
    double *operator*(double *arr)
    {
        double *res = new double[height];
        for (int i = 0; i < height; i++)
            res[i] = ScalarProduct(ptr[i], arr, width);
        return res;
    }
};

template <class T1>
class myMatrix : public BaseMatrix
{
public:
    myMatrix(T1 Height = 2, T1 Width = 2) : BaseMatrix(Height, Width)
    {
        cout << "myMatrix constructor is working" << endl;
    }

    ~myMatrix()
    {
        cout << "myMatrix destructor is working" << endl;
    }
    void randomValues()
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                ptr[i][j] = rand(); //(18 * i) % 5 + (64 * j) % 5;
            }
        }
    }
    pair<T1, T1> getCoorOfWeigth()
    {
        T1 x, y, divider;
        divider = x = y = 0;
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                x += ptr[i][j] * (i + 1);
                y += ptr[i][j] * (j + 1);
                divider += ptr[i][j];
            }
        }
        x /= divider;
        y /= divider;
        return (make_pair(x, y));
    }
    friend ostream &operator<<(ostream &ustream, myMatrix &obj)
    {

        ustream << obj.height << " " << obj.width << '\n';
        for (int i = 0; i < obj.height; i++)
        {
            for (int j = 0; j < obj.width; j++)
            {
                ustream << obj.ptr[i][j] << " ";
            }
            ustream << "\n";
        }
        return ustream;
    }
    friend istream &operator>>(istream &ustream, myMatrix &obj)
    {
        T1 temp;

        int height, width;
        ustream >> height >> width;
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                ustream >> temp;
                obj.ptr[i][j] = temp;
            }
        }
        return ustream;
    }
};

int main()
{
    BaseMatrix a(2, 2);
    myMatrix<int> b(3, 3);
    myMatrix<int> q(3, 3);
    q.randomValues();
    pair<double, double> coor;
    b.randomValues();
    coor = b.getCoorOfWeigth();
    cout << coor.first << " " << coor.second << endl;
    q.print();
    b.print();
    ofstream fout("test.txt");
    if (fout)
    {
        try
        {
            fout << q;
            fout << b;
            fout.close();
        }
        catch (...)
        {
            cout << "Exception: failure of writing";
        }
    }
    ifstream fin("test.txt");
    myMatrix<int> empty_b(3, 3);
    myMatrix<int> empty_q(3, 3);
    if (fin)
    {
        try
        {
            while (!fin.eof())
            {
                int pos = fin.tellg();
                int h, w;
                fin >> h >> w;
                myMatrix<int> temp(h, w);
                fin.seekg(pos, fin.beg);
                fin >> temp;
                temp.print();
            }
            fin.close();
        }
        catch (...)
        {
            cout << "Exception: failure of reading";
        }
    }
    try
    {
        BaseMatrix a(2, 2);
        a(2, -1) = 123;
    }
    catch (IndexOutOfBounds exc)
    {
        exc.print();
    }
    catch (WrongSizeException exc)
    {
        exc.print();
    }
    catch (...)
    {
        cout << "Something went wrong!" << endl;
    }

    return 0;
}