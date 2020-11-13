#include <iostream>
#include <memory>
#include <vector>
#include <set>
#include <algorithm>
#include <string>



int main()
{

/***************************************************************/
//������� 1.
	/*
	���������� ������ ������� ��� ������ ����� ������������������� (vector, list, deque, set � ����������� �������), ������� ����� ���������:
	�	��� ������� ������ ����,
	�	��� � ��������� �� ������� ������ ���� (��������� ������������� ����������� => � ���� ������ ������� �������� �������� �� ������)
	���������: if constexpr
	*/
	{

	}

/***************************************************************/
//������� 2.
	/* ���������� ������ ������� �������� ���� ��������.
	���� ������ ��������� �������� ��������, �� ��� �������� ������� ����� ��������� �� �������� ������� ���������. ��� ���� �������� ������� � ������ �������� ������ ���� ������ � ���� �� ����.
	���������: if constexpr, is_same
	*/
	{

	}


	/***************************************************************/
//������� 3.
	/* 	���������� ������ ������� ������ �� ������ �������� ��������� ������ �������� (stack, queue, priority_queue)
	���������: if constexpr, is_same
	������������� ����� ��������, ���� � �������� �������� ���������.
	*/
	{

	}

/***************************************************************/
//������� 4.
	{
		//��� ������ ��������� ���� string
		std::string strings[] = {"abc", "123", "qwerty", "#$%"};
		//�� ���������� ��������� ������ ������ ������������ � ������������ ����������.
		//��������� ���������� ��������������� �������� �) ��� ����������� � �) ��� ��������� �������
		//��������� � �������!

		//� std::set "����������" �� �������� ������� ��� �����, ������� �������� ������ �����


		__asm nop
		/******************************************************************************************/

		//� std::vector "����������" ������� ��� �����, ������� �������� ������ �����
		//������� �� �����
		//������� �����

		//std::vector<std::shared_ptr < std::string>>

		/******************************************************************************************/
		//���� "����������" ������� ��� �����, ������� �� �������� �� �������� ����, �� �������� ����
		//� ������ �������


	}


/******************************************************************************************/
//������� 5.
	{
		//����:
		std::string ar[] = {"my","Hello", "World"};
		std::vector < std::shared_ptr<std::string>> v = {std::make_shared<std::string>("good"), std::make_shared<std::string>("bye")};



		//�) ��������� �������� � ������ ������� ��� ��������� �������, �� ������� �������� �������!
		//�) ������������� ������ �� �������� � ������� �� �����
		//�) ���������� ���������� ������������ ������


		__asm nop
	}
	/***************************************************************/
		//������� 6. shared_ptr � weak_ptr
		//������� ��������������� ������ ����������� ������ human. � ������ ��������:
		//��� - string
		//�������� �������: ��� ��� ��� ���...
		//�������� - shared_ptr (�������� �� ������ ��������...)
		//���� - ��������� �� weak_ptr (����� �������� ����������� ������������)

		//������ ������ human:
		//����������� - ��� ������������� ����� � ��������
		//����������� �����������, �������� ������������, move ???
		//����������� ����� child() -
		//				������ ������� ������� � ������� ������� ��� ����������� ��������
		//				+ ������������ ��� ����� ������� � ���������� � ��������

		//������ ����������� ����������� ��������������� ������ ��� ���������� ����������

	{
		//������� ������ � ����-�� ���������� => "����-���� ��� �� ����, ��������, ���� � ���"
		//(�� ���� �� ����� ���� ��� ���� � ��� ����):


		//std::shared_ptr<human> grandM1(new human("Eva"));
		//...

		//� ��� ��������� ���� - child():


		//� � ����� � ���� ������� ���� ����:


		//...
		__asm nop
	}



//***************************************************************/
//������� 7.

	/*���������������� deduction guide � ��� ������ ����� ���������� �������
	����� ������ ������, ������� ������������� ���������� ������������ ������ ���������
	����������� � ���������� ������ ����. */
/*
template<typename T, size_t size> class MyArray
	{
		T ar[size]; //��� ���������� ������������� ��������� �������� ���� �� ��������� �����?
		�
	};

*/
/*
//��������� ���������� ����������������� ����������� �������� �������������.
	{
		MyArray<int, 5> ar1;//MyArray<int,5>
		MyArray ar2{"ABC"}; //MyArray<char,4>
		int ar[] = { 1,2,3 };
		MyArray ar3{ ar };

	}
*/

	__asm nop

}