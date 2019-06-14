#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QString>
#include <math.h>
#define F_NUMBER 4

typedef double (*PFunction)(double, double);

struct surface_data
{
    PFunction func;
    double x_min;
    double x_max;
    double z_min;
    double z_max;
    double d_x;
    double d_z;
};

class Function
{
public:
    Function()
    {
        f_array[0] = &f1;
        f_array[1] = &f2;
        f_array[2] = &f3;
        f_array[3] = &f4;
    }

    PFunction get_function(int index)
    {
        return f_array[index];
    }
    QString get_function_name(int index)
    {
        switch (index) {
        case 1:
            return "y = cos(x) * cos(z)";
        case 2:
            return "y = cos(sqrt(x^2 + z^2))";
        case 3:
            return "y = cos(x + z)";
        case 4:
            return "y = cos(x) + cos(z)";
        default:
            break;
        }
        return "";
    }

private:
    PFunction f_array[F_NUMBER];

    static double f1(double x, double z)
    {
        return cos(x) * cos(z);
    }
    static double f2(double x, double z)
    {
        return cos(sqrt(x * x + z * z));
    }
    static double f3(double x, double z)
    {
        return cos(x + z);
    }
    static double f4(double x, double z)
    {
        return cos(x) + cos(z);
    }
};


#endif // FUNCTIONS_H
