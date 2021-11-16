#ifndef TCL_MATRIX_H
#define TCL_MATRIX_H
#pragma once
#include<iostream>
#include<cmath>
#include<iomanip>
#pragma warning(disable:4996)
#define PRECISION 1e-5

class Matrix
{
private:
    int rank;
public:
    int row;
    int col;
    double** m;

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
    /// ��˹��Ԫ����Ϊ�м򻯽�����
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
        if (col > row)
            rank = row - zeroRow;
        else rank = col - zeroRow;
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
    /// ������Ϊ������󣬿��ô˺������
    /// </summary>
    /// <param name="solution">���ڽ���һ��������</param>
    /// <returns>������ֵΪfalse���򷽳��޽⣬δ�Է��ز�������</returns>
    bool GetAnswerForAugmentedMatrix(Matrix& ret)
    {
        Matrix original(*this);
        int zeroRow = 0;
        for (int i = 0; i < original.col - 1 && i < original.row; i++)  //i�����У���ʱ���������i��ൽcol - 1
        {
            int nonZero = original.row - 1;  //���������Ϊ���һ��
            for (int j = i - zeroRow; j < original.row; j++) // �ҵ���0Ԫ��
                if (abs(original.m[j][i]) > PRECISION)
                {
                    nonZero = j;
                    break;
                }

            if (abs(original.m[nonZero][i]) > PRECISION)
            {
                if (nonZero != i - zeroRow) //�����0Ԫ�ز��ǵ�i-zeroRow��
                {
                    for (int k = i; k < original.col; k++) // �ѷ�0Ԫ�������н�������ǰ��
                    {
                        double t = original.m[i - zeroRow][k];
                        original.m[i - zeroRow][k] = original.m[nonZero][k];
                        original.m[nonZero][k] = t;
                    }
                    nonZero = i - zeroRow;
                }

                if (abs(original.m[i - zeroRow][i] - 1.0) > PRECISION)  //��һ��
                {
                    double temp = original.m[i - zeroRow][i];
                    for (int k = i; k < original.col; k++)
                    {
                        original.m[i - zeroRow][k] /= temp;
                    }
                }

                for (int j = i - zeroRow + 1; j < original.row; j++) //�����沿����Ϊ0
                {
                    if (abs(original.m[j][i]) > PRECISION) //���m[j][i]����0
                    {
                        double temp = original.m[j][i];
                        for (int k = i; k < original.col; k++)
                        {
                            original.m[j][k] = original.m[j][k] - original.m[i - zeroRow][k] * temp;
                        }
                    }
                }

                for (int j = i - zeroRow - 1; j >= 0; j--) //�����沿����Ϊ0
                {
                    if (abs(original.m[j][i]) > PRECISION)
                    {
                        double temp = original.m[j][i];
                        for (int k = i; k < original.col; k++)
                        {
                            original.m[j][k] = original.m[j][k] - original.m[i - zeroRow][k] * temp;
                        }
                    }
                }
            }
            else
            {
                zeroRow++;
            }
        }
        if (original.col - 1 > original.row)
            original.rank = original.row - zeroRow;
        else original.rank = original.col - 1 - zeroRow;
        if (original.col - 1 > original.row && original.rank < original.row) //����ʱԭ�������ȣ���col - 1>row�������һ�п��ܲ��ᱻ��һ���������һ�е��Ȳ��ᱻ���㣬Ҫ��������
        {
            int k = original.row;
            while (abs(original.m[original.row - 1][k]) < PRECISION && k < original.col - 1)
            {
                k++;
            }
            if (k < original.col - 1)
            {
                double temp = original.m[original.row - 1][k];
                for (int i = k; i < original.col; i++)
                {
                    original.m[original.row - 1][i] /= temp;
                }
                original.rank++; //�������һ�е���
                for (int j = original.row - 2; j >= 0; j--) //�����沿����Ϊ0
                {
                    if (abs(original.m[j][k]) > PRECISION)
                    {
                        double temp = original.m[j][k];
                        for (int t = k; t < original.col; t++)
                        {
                            original.m[j][t] = original.m[j][t] - original.m[original.row - 1][t] * temp;
                        }
                    }
                }
            }
        }

        //�ȿ���û�н⣬�����м򻯽����͵�ȫ���ж�Ӧ������������е�Ԫ���Ƿ�Ϊ0
        for (int i = original.row - 1; i >= original.rank; i--)
        {
            if (abs(original.m[i][original.col - 1]) > PRECISION)
            {
                return false;
            }
        }

        Matrix solution(col - 1, 1);
        int numOfFreeColumn = 0;

        int temp;  //��¼��
        for (temp = 0; temp < original.col - 1 && temp - numOfFreeColumn < original.row; temp++)  //����λ�ö�Ӧ����ΪĿ�������Ķ�Ӧ�����������ж�Ӧ����ֱ��ȡ0
        {
            if (abs(original.m[temp - numOfFreeColumn][temp]) > PRECISION)
            {
                solution.m[temp][0] = original.m[temp - numOfFreeColumn][original.col - 1];
            }
            else
            {
                numOfFreeColumn++;
                solution.m[temp][0] = 0;//������λ�ô�ȡ0�϶����㷽��
            }
        }

        while (temp < original.col - 1)
        {
            solution.m[temp++][0] = 0;
        }
        ret = solution;
        return true;
    }

    /// <summary>
    /// ������Ϊ������󣬿��ô˺����ҵ����Է���������н�
    /// </summary>
    /// <param name="ret">ǰcol-1������ռ��һ��������1����һ���ؽ�</param>
    /// <returns>������ֵΪfalse���򷽳��޽⣬δ�Է��ز�������</returns>
    bool GetAllSolutionsForAugmentedMatrix(Matrix& ret)
    {
        Matrix original(*this);
        int zeroRow = 0;
        for (int i = 0; i < original.col - 1 && i < original.row; i++)  //i�����У���ʱ���������i��ൽcol - 1
        {
            int nonZero = original.row - 1;  //���������Ϊ���һ��
            for (int j = i - zeroRow; j < original.row; j++) // �ҵ���0Ԫ��
                if (abs(original.m[j][i]) > PRECISION)
                {
                    nonZero = j;
                    break;
                }

            if (abs(original.m[nonZero][i]) > PRECISION)
            {
                if (nonZero != i - zeroRow) //�����0Ԫ�ز��ǵ�i-zeroRow��
                {
                    for (int k = i; k < original.col; k++) // �ѷ�0Ԫ�������н�������ǰ��
                    {
                        double t = original.m[i - zeroRow][k];
                        original.m[i - zeroRow][k] = original.m[nonZero][k];
                        original.m[nonZero][k] = t;
                    }
                    nonZero = i - zeroRow;
                }

                if (abs(original.m[i - zeroRow][i] - 1.0) > PRECISION)  //��һ��
                {
                    double temp = original.m[i - zeroRow][i];
                    for (int k = i; k < original.col; k++)
                    {
                        original.m[i - zeroRow][k] /= temp;
                    }
                }

                for (int j = i - zeroRow + 1; j < original.row; j++) //�����沿����Ϊ0
                {
                    if (abs(original.m[j][i]) > PRECISION) //���m[j][i]����0
                    {
                        double temp = original.m[j][i];
                        for (int k = i; k < original.col; k++)
                        {
                            original.m[j][k] = original.m[j][k] - original.m[i - zeroRow][k] * temp;
                        }
                    }
                }

                for (int j = i - zeroRow - 1; j >= 0; j--) //�����沿����Ϊ0
                {
                    if (abs(original.m[j][i]) > PRECISION)
                    {
                        double temp = original.m[j][i];
                        for (int k = i; k < original.col; k++)
                        {
                            original.m[j][k] = original.m[j][k] - original.m[i - zeroRow][k] * temp;
                        }
                    }
                }
            }
            else
            {
                zeroRow++;
            }
        }
        if (original.col - 1 > original.row)
            original.rank = original.row - zeroRow;  //������ָԭ������ȣ��������������ȡ�
        else original.rank = original.col - 1 - zeroRow;
        if (original.col - 1 > original.row && original.rank < original.row) //����ʱԭ�������ȣ���col - 1>row�������һ�п��ܲ��ᱻ��һ���������һ�е��Ȳ��ᱻ���㣬Ҫ��������
        {
            int k = original.row;
            while (abs(original.m[original.row - 1][k]) < PRECISION && k < original.col - 1)
            {
                k++;
            }
            if (k < original.col - 1)
            {
                double temp = original.m[original.row - 1][k];
                for (int i = k; i < original.col; i++)
                {
                    original.m[original.row - 1][i] /= temp;
                }
                original.rank++; //�������һ�е���
                for (int j = original.row - 2; j >= 0; j--) //�����沿����Ϊ0
                {
                    if (abs(original.m[j][k]) > PRECISION)
                    {
                        double temp = original.m[j][k];
                        for (int t = k; t < original.col; t++)
                        {
                            original.m[j][t] = original.m[j][t] - original.m[original.row - 1][t] * temp;
                        }
                    }
                }
            }
        }
        //�ȿ���û�н⣬�����м򻯽����͵�ȫ���ж�Ӧ������������е�Ԫ���Ƿ�Ϊ0
        for (int i = original.row - 1; i >= original.rank; i--)
        {
            if (abs(original.m[i][original.col - 1]) > PRECISION)
            {
                return false;
            }
        }

        Matrix solution(original.col - 1, original.col - original.rank);
        int numOfFreeColumn = 0;
        int temp = 0;
        for (temp = 0; temp < original.col - 1 && temp - numOfFreeColumn < original.row; temp++) //������ռ�Ļ�
        {
            if (abs(original.m[temp - numOfFreeColumn][temp]) > PRECISION)
                continue;
            else
            {
                solution.m[temp][numOfFreeColumn] = 1;
                for (int j = 0; j < temp - numOfFreeColumn; j++)
                {
                    solution.m[j][numOfFreeColumn] = -original.m[j][temp];
                }
                numOfFreeColumn++;
            }
        }
        while (temp < original.col - 1)
        {
            solution.m[temp][numOfFreeColumn] = 1;
            for (int j = 0; j < original.row; j++)
            {
                solution.m[j][numOfFreeColumn] = -original.m[j][temp];
            }
            numOfFreeColumn++;
            temp++;
        }

        numOfFreeColumn = 0;
        //���ؽ�
        for (temp = 0; temp < original.col - 1 && temp - numOfFreeColumn < original.row; temp++)  //����λ�ö�Ӧ����ΪĿ�������Ķ�Ӧ�����������ж�Ӧ����ֱ��ȡ0
        {
            if (abs(original.m[temp - numOfFreeColumn][temp]) > PRECISION)
            {
                solution.m[temp][original.col - original.rank - 1] = original.m[temp - numOfFreeColumn][original.col - 1];
            }
            else
            {
                numOfFreeColumn++;
                solution.m[temp][original.col - original.rank - 1] = 0;
            }
        }
        while (temp < original.col - 1)
        {
            solution.m[temp++][original.col - original.rank - 1] = 0;
        }
        ret = solution;
        return true;
    }

    /// <summary>
    /// �ҵ���ռ��һ���
    /// </summary>
    /// <param name="bases">���ڷ��ػ�</param>
    /// <returns>��������棬�򷵻�false�����򣬷���һ�����ע�����۾����Ƿ���棬��������Ϊ����ֵ�Ĳ���bases���ᷢ���仯��</returns>
    bool GetBasesOfNullSpace(Matrix& bases)
    {
        Matrix ret(row, col + 1);
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                ret.m[i][j] = m[i][j];
            }
        }
        if (ret.GetAllSolutionsForAugmentedMatrix(ret))
        {
            if (ret.col > 1)
            {
                Matrix nullSpace(ret.row, ret.col - 1);
                for (int i = 0; i < nullSpace.row; i++)
                {
                    for (int j = 0; j < nullSpace.col; j++)
                    {
                        nullSpace.m[i][j] = ret.m[i][j];
                    }
                }
                bases = nullSpace;
                return true;
            }
            else
            {
                std::cout << "The matrix's null space only contains 0 so that it has no bases." << std::endl;
                return false;
            }
        }
        else
        {
            std::cout << "Something went wrong so that the matrix's null space is empty!" << std::endl;
            return false;
        }
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
        rank = A.rank;
        if (A.col != col || A.row != row)
        {
            for (int i = 0; i < row; i++)
                delete[] m[i];
            delete[] m;
            row = A.row;
            col = A.col;
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

    /// <summary>
    /// ������-1��������rankδ����
    /// </summary>
    /// <returns></returns>
    int GetRank()
    {
        return rank;
    }

    /// <summary>
    /// ��ӡ����
    /// </summary>
    /// <param name="width">ָ�������ȣ�Ĭ��Ϊ3</param>
    /// <param name="precision">ָ��������ȣ�Ĭ��Ϊ3</param>
    void Display(int width = 3, int precision = 3)
    {
        for (int i = 0; i < row; i++)
            for (int j = 0; j < col; j++)
            {
                std::cout << std::setprecision(precision) << std::setw(width) << m[i][j] << " ";
                if (j == col - 1)
                    std::cout << std::endl;
            }
    }
};
#endif