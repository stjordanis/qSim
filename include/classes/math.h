#ifndef CLASSES_MATH_H
#define CLASSES_MATH_H

namespace qsim {
    namespace math {

        extern const std::complex<double> I(0,1);
        extern const double pi = std::atan(1) * 4;

        template <typename T> const char* sgn(T val) {
            return (val > 0) ? "" : "-";
        };

        class Matrix {
            private:
                std::vector<std::vector<std::complex<double>>> data;
                std::vector<std::vector<std::complex<double>>>::size_type ySize;
                std::vector<std::complex<double>>::size_type xSize;

            public:


                Matrix() {};


                Matrix(std::initializer_list<std::vector <std::complex <double> > > d) : data(d) {
                    ySize = data.size();
                    xSize = data[0].size();
                    
                };

                Matrix(unsigned int n, unsigned int m) {
                    setSize(n,m);
                };
                Matrix(unsigned int n) {
                    setSize(n,n);
                };

                void setSize(unsigned int n, unsigned int m) {
                    xSize = n;
                    ySize = m;
                    data.resize(ySize, std::vector<std::complex <double> >(xSize));
                };
                
                void setValue(unsigned int y, unsigned int x, const std::complex<double> value) {
                    data[y][x] = value;
                };

                std::vector<std::vector<std::complex<double>>>::size_type getYSize() const {
                    return ySize;
                };

                std::vector<std::complex<double>>::size_type getXSize() const {
                    return xSize;
                };
                
                std::vector<std::vector<std::complex<double>>> getData() const {
                    return data;
                };

                std::complex<double> getValue(unsigned int y, unsigned int x) const {
                    return data[y][x];
                };


                void print() {
                    for (int j = 0; j < ySize; j++)
                    {
                        std::cout.precision(3);
                        std::cout << "|";

                        for (int i = 0; i < xSize; i++)
                        {
                            if (std::imag(data[j][i]) == 0)
                            {
                            
                                std::cout << " " << std::real(data[j][i]) << " ";
                            }
                            else if (real(data[j][i]) == 0)
                            {
                                if (std::abs(std::imag(data[j][i])) == 1.) {
                                    std::cout << " " << sgn(std::imag(data[j][i])) << "i ";
                                } else {
                                    std::cout << " " << std::imag(data[j][i]) << "i ";
                                };
                            }
                            else
                            {
                                if (std::abs(std::imag(data[j][i])) == 1.)
                                {
                                    std::cout << " " << std::real(data[j][i]) << " + "
                                              << " " << sgn(std::imag(data[j][i])) << "i ";
                                } else {
                                    std::cout << " " << std::real(data[j][i]) << " + " << std::imag(data[j][i]) << "i ";
                                };
                            };
                        };

                        std::cout << "|\n";
                    };

                    std::cout << std::endl;
                };

                Matrix adjoint() {
                    Matrix res(xSize, ySize);

                    for (int i = 0; i < ySize; i++){
                        for (int j = 0; j < xSize; j++) {
                            res.setValue(j, i, std::conj(data[i][j]));
                        };
                    };

                    return res;
                };

                Matrix controlled() {
                    Matrix res(xSize * 2, ySize * 2);
                    for (int j = 0; j < ySize; j++){
                        res.setValue(j, j, 1);

                        for (int i = 0; i < xSize; i++){
                            res.setValue(ySize + j, xSize + i, data[j][i]);
                        };
                    };
                    return res;
                }

                Matrix operator*=(Matrix const &mat) {
                    Matrix res(xSize, ySize);
                    std::complex<double> midresult = 0;
                    if (xSize != mat.getYSize()) {
                        throw std::runtime_error("You're trying to multiply Matrices of incompatible sizes!");
                    } else {
                        for (int j = 0; j < ySize; j++)
                        {
                            for (int i = 0; i < mat.getXSize(); i++)
                            {
                                for (int k = 0; k < mat.getYSize(); k++) {
                                    midresult += (data[i][k] * mat.getValue(k,j));
                                };
                                res.setValue(j, i, midresult);
                                midresult = 0;
                            };
                        };
                    };
                    *this = res;
                    return *this;
                };
        };

        Matrix operator*(Matrix mat1, Matrix mat2) {
            return mat1 *= mat2;
        };

        // 2 matrix tensor product
        // test this pls
        Matrix tensorProduct (Matrix mat1, Matrix mat2) {
            Matrix res(mat1.getXSize() * mat2.getXSize(), mat1.getYSize() * mat2.getYSize());

            for (int y1 = 0; y1 < mat1.getYSize(); y1++) {
                for (int x1 = 0; x1 < mat1.getXSize(); x1++) {
                    for (int y2 = 0; y2 < mat2.getYSize(); y2++) {
                        for (int x2 = 0; x2 < mat2.getXSize(); x2++) {
                            res.setValue(mat2.getYSize() * y1 + y2, mat2.getXSize() * x1 + x2, mat1.getValue(y1, x1) * mat2.getValue(y2, x2));
                        };
                    };
                };
            };
            return res;
        }

        class Ket
        {
            private:
                std::vector<std::complex<double>> data;
                std::vector<std::complex<double>>::size_type size;

            public:
                Ket(){

                };

                Ket(std::initializer_list<std::complex<double>> d) : data(d)
                {
                    size = d.size();
                };

                Ket(unsigned int n)
                {
                    setSize(n);
                };

                void setSize(unsigned int n)
                {
                    size = n;
                    data.resize(n);
                };

                void setValue(unsigned int index, const std::complex<double> value) {
                    data[index] = value;
                }


                std::complex<double> getValue(unsigned int index) const {
                    return data[index];
                }

                std::vector<std::complex<double>> getData() const {
                    return data;
                }

                std::vector<std::complex<double>>::size_type getSize() const {
                    return size;
                };

                void print()
                {
                    std::cout << std::endl;

                    for (int j = 0; j < size; j++)
                    {
                        std::cout.precision(3);
                        if (imag(data[j]) == 0)
                        {

                            std::cout << "| " << real(data[j]) << " |\n";
                        }
                        else if (real(data[j]) == 0)
                        {
                            std::cout << "| " << imag(data[j]) << "i |\n";
                        }
                        else
                        {
                            std::cout << "| " << real(data[j]) << " + " << imag(data[j]) << "i |\n";
                        };
                    };

                    std::cout << std::endl;
                };

                /**************/
                /*            */
                /*  Operator  */
                /*  overload  */
                /*            */
                /**************/

                // adding 2 Kets
                Ket operator+(Ket const &obj) const
                {
                    Ket res(size);
                    if (size != obj.getSize())
                    {
                        throw std::runtime_error("You're trying to add Kets of different sizes!");
                    };
                    for (unsigned int i = 0; i < size; i++)
                    {
                        res.setValue(i, data[i] + obj.getValue(i));                    
                    };
                    return res;
                };

                // minusing 2 Kets
                Ket operator-(Ket const &obj) const
                {
                    Ket res(size);

                    if (size != obj.getSize())
                    {
                        throw std::runtime_error("You're trying to add Kets of different sizes!");
                    };
                    for (unsigned int i = 0; i < size; i++)
                    {
                        res.setValue(i, data[i] - obj.getValue(i));
                    };
                    return res;
                };

                // multiplying two kets
                Ket operator*(Ket const &obj) const
                {
                    Ket res(size * obj.getSize());

                    for (unsigned int i = 0; i < size; i++)
                    {
                        for (unsigned int j = 0; j < obj.getSize(); j++)
                        {
                            res.setValue(i * obj.getSize() + j, data[i] * obj.getValue(j));
                        };
                    };
                    return res;
                };

                // dividing a Ket by a number
                Ket operator/(std::complex<double> numerator) const
                {
                    Ket res(size);

                    for (unsigned int i = 0; i < size; i++)
                    {
                        res.setValue(i, data[i] / numerator);
                    };
                    return res;
                };

                // multiplying a Ket by a number
                Ket operator*(std::complex<double> multiplier) const
                {
                    Ket res(size);

                    for (unsigned int i = 0; i < size; i++)
                    {
                        res.setValue(i, data[i] * multiplier);
                    };
                    return res;
                };

                // assign* a Ket with another
                Ket operator*=(Ket const &obj2) {
                    *this = *this * obj2;
                    return *this;
                }

                // assign+ a Ket with another
                Ket operator+=(Ket const &obj2)
                {
                    *this = *this + obj2;
                    return *this;
                };

                // assign- a Ket with another
                Ket operator-=(Ket const &obj2)
                {
                    *this = *this - obj2;
                    return *this;
                };

                // assign* a Ket with a number
                Ket operator*=(std::complex<double> multiplier)
                {
                    *this = *this * multiplier;
                    return *this;
                };

                // assign/ a Ket with a number
                Ket operator/=(std::complex<double> numerator)
                {
                    *this = *this / numerator;
                    return *this;
                };

                // assign* apply a gate onto a Ket
                Ket operator*=(Matrix mat)
                {
                    Ket res(size);
                    std::complex<double> midresult = 0;

                    for (int gY = 0; gY < mat.getYSize(); gY++)
                    {
                        for (int qS = 0; qS < size; qS++)
                        {
                            midresult += (mat.getValue(gY,qS) * data[qS]);
                        };
                        res.setValue(gY, midresult);
                        midresult = 0;
                    };

                    *this = res;
                    return *this;
                };

        };



        // Apply a gate onto a Ket
        Ket operator*(Matrix mat, Ket ket)
        {
            return ket *= mat;
        };


    }; // end namespace math
}; // end namespace qsim

#endif