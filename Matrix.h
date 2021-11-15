#ifndef MATRIX_H
#define MATRIX_H
#pragma once
#include<iostream>
#include<cmath>
#include<iomanip>
#pragma warning(disable:4996)
#define PRECISION 1e-5

class Matrix
{
public:
    int row;
    int col;
    double** m;
    int rank;

    /// <summary>
    /// �����о�����Ƶ���������У���ʼʱ��֪���ȣ�����Ϊ-1��
    /// </summary>
    /// <param name="matrix">ʹ��ʱ�����Խ�double**����ǿתΪdouble*��������</param>
    /// <param name="row"></param>
    /// <param name="col"></param>
    Matrix(double* matrix, int row, int col) :row(row), col(col), rank(-1)
    {
        m = new double* [row];
        for (int i = 0; i < row; i++)
            m[i] = new double[col];
        for (int i = 0; i < row; i++)
            for (int j = 0; j < col; j++)
            {
                m[i][j] = matrix[col * i + j];
            }
    }

    /// <summary>
    /// �����¾��󲢳�ʼ��Ϊ0
    /// </summary>
    /// <param name="row"></param>
    /// <param name="col"></param>
    Matrix(int row = 1, int col = 1) :row(row), col(col), rank(-1)
    {
        if (row < 1)
            row = 1;
        if (col < 1)
            col = 1;
        m = new double* [row];
        for (int i = 0; i < row; i++)
            m[i] = new double[col];
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                m[i][j] = 0;
            }
        }
    }

    /// <summary>
    /// ���ƹ��캯�������
    /// </summary>
    /// <param name="A"></param>
    Matrix(const Matrix& A) : row(A.row), col(A.col), rank(A.rank)
    {
        m = new double* [row];
        for (int i = 0; i < row; i++)
            m[i] = new double[col];
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                m[i][j] = A.m[i][j];
            }
        }
    }

    ~Matrix()
    {
        for (int i = 0; i < row; i++)
            delete[] m[i];
        delete[] m;
    }

    /// <summary>
    /// ��˹��Ԫ
    /// </summary>
    void Gauss_Jordan_Elimination()
    {
        int zeroRow = 0;
        for (int i = 0; i < col && i < row; i++)  //i������
        {
            int nonZero = row - 1;  //���������Ϊ���һ��
            for (int j = i - zeroRow; j < row; j++) // �ҵ���0Ԫ��
                if (abs(m[j][i]) > PRECISION)
                {
                    nonZero = j;
                    break;
                }

            if (abs(m[nonZero][i]) > PRECISION)
            {
                if (nonZero != i - zeroRow) //�����0Ԫ�ز��ǵ�i-zeroRow��
                {
                    for (int k = i; k < col; k++) // �ѷ�0Ԫ�������н�������ǰ��
                    {
                        double t = m[i - zeroRow][k];
                        m[i - zeroRow][k] = m[nonZero][k];
                        m[nonZero][k] = t;
                    }
                    nonZero = i - zeroRow;
                }

                if (abs(m[i - zeroRow][i] - 1.0) > PRECISION)  //��һ��
                {
                    double temp = m[i - zeroRow][i];
                    for (int k = i; k < col; k++)
                    {
                        m[i - zeroRow][k] /= temp;
                    }
                }

                for (int j = i - zeroRow + 1; j < row; j++) //�����沿����Ϊ0
                {
                    if (abs(m[j][i]) > PRECISION) //���m[j][i]����0
                    {
                        double temp = m[j][i];
                        for (int k = i; k < col; k++)
                        {
                            m[j][k] = m[j][k] - m[i - zeroRow][k] * temp;
                        }
                    }
                }

                for (int j = i - zeroRow - 1; j >= 0; j--) //�����沿����Ϊ0
                {
                    if (abs(m[j][i]) > PRECISION)
                    {
                        double temp = m[j][i];
                        for (int k = i; k < col; k++)
                        {
                            m[j][k] = m[j][k] - m[i - zeroRow][k] * temp;
                        }
                    }
                }
            }
            else
            {
                zeroRow++;
            }
        }
        rank = row - zeroRow;
        if (col > row && rank < row) //����ʱ�����ȣ���col>row�������һ�п��ܲ��ᱻ��һ���������һ�е��Ȳ��ᱻ���㣬Ҫ��������
        {
            int k = row;
            while (abs(m[row - 1][k]) < PRECISION && k < col)
            {
                k++;
            }
            if (k < col)
            {
                double temp = m[row - 1][k];
                for (int i = k; i < col; i++)
                {
                    m[row - 1][i] /= temp;
                }
                rank++; //�������һ�е���
                for (int j = row - 2; j >= 0; j--) //�����沿����Ϊ0
                {
                    if (abs(m[j][k]) > PRECISION)
                    {
                        double temp = m[j][k];
                        for (int t = k; t < col; t++)
                        {
                            m[j][t] = m[j][t] - m[row - 1][t] * temp;
                        }
                    }
                }
            }
        }
    }

    /// <summary>
    /// ������ľ�����������������ʹ��Ѱ��һ����
    /// </summary>
    /// <returns>���ⲻ���ڣ��򷵻�NULL�����򣬷���һ���⡣ע�⣬������ֵ��ΪNULL���򷵻ص�ָ����Ҫ�ֶ�delete</returns>
    double* GetAnswerForAugmentedMatrix()
    {
        if (rank < 0)
            return NULL;
        //�ȿ���û�н⣬�����м򻯽����͵�ȫ���ж�Ӧ������������е�Ԫ���Ƿ�Ϊ0
        for (int i = row - 1; i >= rank; i--)
        {
            if (abs(m[i][col - 1]) > PRECISION)
                return NULL;
        }
        double* ret = new double[col - 1];
        int numOfFreeColumn = 0;

        int temp;  //��¼��
        for (temp = 0; temp < col - 1 && temp - numOfFreeColumn < row; temp++)  //����λ�ö�Ӧ����ΪĿ�������Ķ�Ӧ�����������ж�Ӧ����ֱ��ȡ0
        {
            if (abs(m[temp - numOfFreeColumn][temp]) > PRECISION)
            {
                ret[temp] = m[temp - numOfFreeColumn][col - 1];
            }
            else
            {
                numOfFreeColumn++;
                ret[temp] = 0; //������λ�ô�ȡ0�϶����㷽��
            }
        }

        while (temp < col - 1)
        {
            ret[temp++] = 0;
        }
        return ret;
    }

    /// <summary>
    /// ����ʽ����Ϊ����ʱ����0
    /// </summary>
    double Determinant() const
    {
        if (row != col)
            return 0;
        int swapTimes = 0; //�н�������
        double** mtemp = new double* [row];
        for (int i = 0; i < row; i++)
            mtemp[i] = new double[col];
        for (int i = 0; i < row; i++)
            for (int j = 0; j < col; j++)
            {
                mtemp[i][j] = m[i][j];
            }
        int zeroRow = 0;
        for (int i = 0; i < col && i < row; i++)  //i������
        {
            int nonZero = row - 1;  //���������Ϊ���һ��
            for (int j = i - zeroRow; j < row; j++) // �ҵ���0Ԫ��
                if (abs(mtemp[j][i]) > PRECISION)
                {
                    nonZero = j;
                    break;
                }

            if (abs(mtemp[nonZero][i]) > PRECISION)
            {
                if (nonZero != i - zeroRow) //�����0Ԫ�ز��ǵ�i-zeroRow��
                {
                    for (int k = i; k < col; k++) // �ѷ�0Ԫ�������н�������ǰ��
                    {
                        double t = mtemp[i - zeroRow][k];
                        mtemp[i - zeroRow][k] = mtemp[nonZero][k];
                        mtemp[nonZero][k] = t;
                    }
                    nonZero = i - zeroRow;
                    swapTimes++;
                }

                for (int j = i - zeroRow + 1; j < row; j++) //�����沿����Ϊ0
                {
                    if (abs(mtemp[j][i]) > PRECISION) //���mtemp[j][i]����0
                    {
                        double temp = mtemp[j][i] / mtemp[i - zeroRow][i];
                        for (int k = i; k < col; k++)
                        {
                            mtemp[j][k] = mtemp[j][k] - mtemp[i - zeroRow][k] * temp;
                        }
                    }
                }
            }
        }
        double ret = 1;
        if (abs(mtemp[row - 1][col - 1]) < PRECISION)
            ret = 0;
        else
        {
            for (int i = 0; i < row; i++)
            {
                ret *= mtemp[i][i];
            }
            if (swapTimes % 2)
                ret = -ret;
        }

        for (int i = 0; i < row; i++)
            delete[] mtemp[i];
        delete[]mtemp;
        return ret;
    }

    /// <summary>
    /// �����������
    /// </summary>
    /// <param name="inverse">inverseӦΪrow*row�ľ���������ڽ��շ���ֵ����������棬�������ͨ���ò������أ������󲻿��棬��inverse��������κβ���</param>
    /// <returns>���ؾ����Ƿ����</returns>
    bool InverseMatrix(Matrix& inverse)
    {
        if (row != col)
            return false;
        if (inverse.row != inverse.col || inverse.row != row)
        {
            std::cout << "The matrix may be invertible, but the second parameter has wrong dimension so it failed to receive the returned value!" << std::endl;
            return false;
        }
        Matrix caculateInverse(row, 2 * row);
        for (int i = 0; i < row; i++)
            for (int j = 0; j < row; j++)
            {
                caculateInverse.m[i][j] = m[i][j];
                caculateInverse.m[i][j + row] = i == j ? 1 : 0;
            }
        caculateInverse.Gauss_Jordan_Elimination();
        if (abs(caculateInverse.m[row - 1][row - 1]) < PRECISION) //��˹��Ԫ�����½�Ϊ0���򲻿��棻������档
            return false;
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < row; j++)
            {
                inverse.m[i][j] = caculateInverse.m[i][j + row];
            }
        }
        return true;
    }

    /// <summary>
    /// ����˷�
    /// </summary>
    /// <param name="A"></param>
    /// <param name="B"></param>
    /// <returns>��ά��ʧ�䣬�򷵻ص�һ��������</returns>
    friend Matrix operator *(const Matrix& A, const Matrix& B)
    {
        if (A.col != B.row)
        {
            std::cout << "The dimension does not match! Matrix multiplication failed. Return the first matrix." << std::endl;
            return A;
        }
        Matrix ret(A.row, B.col);
        for (int i = 0; i < ret.row; i++)
        {
            for (int j = 0; j < ret.col; j++)
            {
                for (int k = 0; k < A.col; k++)
                {
                    ret.m[i][j] += A.m[i][k] * B.m[k][j];
                }
            }
        }
        return ret;
    }

    /// <summary>
    /// ����ӷ�
    /// </summary>
    /// <param name="A"></param>
    /// <param name="B"></param>
    /// <returns>��ά��ʧ�䣬�򷵻ص�һ��������</returns>
    friend Matrix operator +(const Matrix& A, const Matrix& B)
    {
        if (A.col != B.col || A.row != B.row)
        {
            std::cout << "The dimension does not match! Matrix addtion failed. Return the first matrix." << std::endl;
            return A;

        }
        Matrix ret(A.row, B.row);
        for (int i = 0; i < ret.row; i++)
        {
            for (int j = 0; j < ret.col; j++)
            {
                ret.m[i][j] += A.m[i][j] + B.m[i][j];
            }
        }
        return ret;
    }

    /// <summary>
    /// �������
    /// </summary>
    /// <param name="A"></param>
    /// <param name="B"></param>
    /// <returns>��ά��ʧ�䣬�򷵻ص�һ��������</returns>
    friend Matrix operator -(const Matrix& A, const Matrix& B)
    {
        if (A.col != B.col || A.row != B.row)
        {
            std::cout << "The dimension does not match! Matrix subtraction failed. Return the first matrix." << std::endl;
            return A;
        }
        Matrix ret(A.row, B.row);
        for (int i = 0; i < ret.row; i++)
        {
            for (int j = 0; j < ret.col; j++)
            {
                ret.m[i][j] += A.m[i][j] - B.m[i][j];
            }
        }
        return ret;
    }

    Matrix operator =(const Matrix& A)
    {
        if (A.col != col || A.row != row)
        {
            for (int i = 0; i < row; i++)
                delete[] m[i];
            delete[] m;
            row = A.row;
            col = A.col;
            rank = A.rank;
            m = new double* [row];
            for (int i = 0; i < row; i++)
                m[i] = new double[col];
        }
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                m[i][j] = A.m[i][j];
            }
        }
        return *this;
    }

    /// <summary>
    /// ������ݣ��������Ϊ����
    /// </summary>
    /// <param name="n">��������Ȼ��</param>
    /// <returns>������Ϊ��������С��0���򷵻�ԭ����</returns>
    Matrix Power(int n)
    {
        if (row != col)
        {
            std::cout << "The matrix is not a square!" << std::endl;
            return *this;
        }
        else if (n < 0)
        {
            std::cout << "The specified power is less than 0! No changes!" << std::endl;
            return *this;
        }
        else
        {
            Matrix ori(*this), ret(row, row);
            for (int i = 0; i < row; i++)
                ret.m[i][i] = 1;
            while (n > 0)
            {
                if (n % 2)
                {
                    ret = ret * ori;
                    n -= 1;
                    ori = ori * ori;
                    n /= 2;
                }
                else
                {
                    ori = ori * ori;
                    n /= 2;
                }
            }
            return ret;
        }
    }

    void Display(int width = 3, int precision = 3)
    {
        for (int i = 0; i < row; i++)
            for (int j = 0; j < col; j++)
            {
                std::cout << std::setprecision(precision) << std::setw(width) << m[i][j] << " ";
                if (j == col - 1)
                    std::cout << std::endl;
            }
        std::cout << std::endl;
    }
};
#endif