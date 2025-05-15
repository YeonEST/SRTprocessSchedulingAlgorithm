#include<iostream>
using namespace std;
/*SRTF �˰����� ť�� ���� ���� �� ���μ����� ������ ��� ���μ��� �� ���� ª�� �ð��� �䱸�ϴ�
���α׷��� ���� �����ϸ� ���� �߿����� �񱳿����� �Ź� �Ͼ �������� ���μ������� ª�� �ð���
�䱸�ϴ� ���μ����� �Է� �� �ÿ� �����ȴ�.*/

#define IM 5 // �Ӱ谪���� ������ if������ �� ������ ���� remain time�� ������ ���� ������ �ʰ� ����

struct process {/*���μ��� ������ ���� Id�� ���� �ð� ������ ���� ����ü�� ����
				�� �ð����� ���ǿ��� �����Բ��� �˷��ֽ� �ð� �׷����� ���� �ð����� �����*/
	int ID; // ���μ��� ID
	int AT; // Arrival time ���� �׽�Ʈ �� TRT����� ���� �����ð�
	int ST; // Start time
	int BT; // Bust time
	int RT; // Remain time ���� �ð����� BT - ����ð� ���̸� 0�� �Ǹ� ���α׷� �Ϸ�
	int CT; // Completion time �Ϸ� �ð�
	int WT; // Waiting time ť���� ����� �ð�
	int RST; // Respoon time => ����ð� ���ۺ��� ù ��±��� �ɸ� �ð�, Ȥ�� ������� �ɸ��ð����� ST - AT
	int TT; // Turnaround time => ���ð� + ����ƮŸ�� or CT - AT
};

int main() {
	int num; // ���μ��� ������ �Է¹��� ����,
	int currentTime = 0; // �� ���α׷�(�˰���)�� �ð� ī��Ʈ
	int totalTT, totalWT, totalRST;// ������� float avg~������ ��� ���ð�, ��ȯ�ð�, ����ð��� ���ϱ� ���� ���� ����
	totalTT = 0; totalWT = 0; totalRST = 0;
	float avgTT, avgWT, avgRST; avgTT = 0; avgWT = 0; avgRST = 0;
	struct process prc[200]; // �ִ� ���μ��� ������ 200���� ����
	bool complete_flag[200] = { true, }; // true�� �� �صΰ� �Է¹��� ������ŭ 0~(n-1)���� false�� �ٲ�

	/*���μ��� ���� �Է� ����*/
	cout << "���μ��� ���� : ";
	cin >> num;
	cout << endl;
	for (int i = 0; i < num; i++) {
		prc[i].ID = i; // ���μ��� ���̵� ����
		complete_flag[i] = false;
		cout << "���μ��� " << prc[i].ID << "�� �����ð� : ";
		cin >> prc[i].AT;
		cout << "���μ��� " << prc[i].ID << "�� ����Ʈ�ð� : ";
		cin >> prc[i].BT;
		prc[i].RT = prc[i].BT; // ���� ����ƮŸ���� ���� ���� �� �Ǿ����� �䱸 ����ƮŸ�Ӱ� ����
		cout << endl;
	}
	/*���μ��� ���� �Է� ����*/
	bool temp = 0; // �ݺ� �˻縦 ���� ����
	while (1) {
		int runID = -1; // ������ ���μ����� ���̵�, �ʱⰪ�� -1�� ����
		/*�ݺ� ���� �˻� ����*/
		for (int i = 0; i < num; i++) {
			if (complete_flag[i] == false) {
				temp = 0; // 1���� �Ϸᰡ �� �� ���μ����� ������ temp�� 0�� ��
				break;
			}
			else temp = 1; // ��� �Ϸ�Ǿ����� temp�� 1�� ��
		}
		if (temp == 1) break; //  temp 2�̸� ��� �Ϸ�Ǿ������� �ݺ����� ����ǰ� ��
		/*�ݺ� ���� �˻� ����*/
		/*�����ٸ� ����*/
		int m = 9999999; // ���� �ð� �񱳿� ����ϱ� ���� ���� üũ���� ���μ����� ���� ����ƮŸ�� ����� ���� ������, �ʱ� ���� ũ�� �����ص�
		for (int i = 0; i < num; i++) {
			if (prc[runID].RT <= IM && runID != -1) // ù ����ÿ��� runID���� -1�̱� ������ ������ �߻��Ͽ� ������ �߻����� �ʰ� if���ǿ� runID != -1�� �߰�
				break; // ���� �������� ���μ����� ���� ����ƮŸ���� �Ӱ԰�(IM = 5) ���ϸ� ���������� �ʰ� ��
			if (prc[i].AT <= currentTime && complete_flag[i] == 0) { // ���α׷� �����ð��� ����ð����� �������� �߰ų� �����ð��� �����ϰ� �Ϸᰡ �� �Ǿ����� �˻�
				if (prc[i].RT < m) { // ���� ���� �Ǿ��ִ� ���μ����� ���� ����ƮŸ�Ӻ��� �� ���� ����(���� ������) ���μ��� �߽߰� ���� �� ���μ����� �ش� ���μ����� ����
					m = prc[i].RT;
					runID = i;
				}
				if (prc[i].RT == m) {
					if (prc[i].AT < prc[runID].AT) { // ���� ����ƮŸ���� ���� ���μ����� �����ϴ� ��� �� ���� ������ ���μ����� ����
						m = prc[i].RT;
						runID = i;
					}
				}
			}
		}

		if (runID != -1) { // ���� �� ���μ��� ������ ����
			if (prc[runID].RT == prc[runID].BT)
				prc[runID].ST = currentTime; // �� ���� ����� �� ���� ���μ����� ù �����̴� ���۽ð� = ����ð����� ���

			prc[runID].RT -= 1; // 1 ��ŭ�� �ð����� ����Ǿ������� �����ð��� -1
			currentTime++; // ����ð� 1 ����

			if (prc[runID].RT == 0) { // ���μ��� ���� �Ϸ�� �Ϸ�ð�, �Ͼ����ð�, ���ð�, ����ð� ���� ����Ͽ� �Է�
				/*���μ��� ���� ���� �Է�*/
				prc[runID].CT = currentTime; // �Ϸ�ð��� ����ð����� ����
				prc[runID].TT = prc[runID].CT - prc[runID].AT; // �Ͼ����Ÿ���� �Ϸ�ð� - �����ð�
				prc[runID].WT = prc[runID].TT - prc[runID].BT; // ���ð��� ����ð� - �����ð�
														//Ȥ�� �Ͼ����Ÿ�� = ���ð�+����ð����� ���ð� = �Ͼ���� - ����ð����� ���� ���⼱ ����
				prc[runID].RST = prc[runID].ST - prc[runID].AT; // ����ð� = ���۽ð� - �����ð�
				/*��հ��� ���� ��Ż �� �Է�*/
				totalTT += prc[runID].TT;
				totalWT += prc[runID].WT;
				totalRST += prc[runID].RST;

				complete_flag[runID] = true; // �ش� ID�� ���μ����� �Ϸ�Ǿ��� ���
			}
		}
		else break; // �߾�ó����ġ�� ������ ���μ����� ������ ����
	}
	/*�����ٸ� ����*/
	/*�����ٸ� ���� ��հ� ��� ����*/
	avgTT = (float)totalTT / num; avgWT = (float)totalWT / num; avgRST = (float)totalRST / num; // total�ڷ���� int���̱� ������ ĳ��Ʈ �����ڷ� floatȭ �����ش�

	int arr1[200], arr2[200]; // ���μ����� �Ϸ� ������� �����ϱ� ���� ���μ��� �ε����� ���� �迭 ����, arr1�� �Ϸ�ð� arr2�� ID
	for (int i = 0; i < num; i++) arr1[i] = prc[i].CT; // arr1�� ���μ��� ID �� �Ϸ�ð� �Է�
	for (int i = 0; i < num; i++) arr2[i] = prc[i].ID; // arr2�� ���μ��� ID �Է�
	for (int i = 0; i < num-1; i++) { // �Ϸ�ð����� ����, �˰����� ���������� �̿��Ͽ���
		int le = i;
		for (int j = i + 1; j < num; j++) {
			if (arr1[le] > arr1[j])
				le = j;
		}
		int temp = arr1[i];
		arr1[i] = arr1[le];
		arr1[le] = temp;

		temp = arr2[i];
		arr2[i] = arr2[le];
		arr2[le] = temp;
	} // ���� ��
	cout << "���μ��� ����" << endl << "���μ��� ID\t�����ð�\t����ƮŸ��\t���ð�\t�Ϸ�ð�" << endl;
	for (int i = 0; i < num; i++)
		cout << prc[i].ID << "\t\t" << prc[i].AT << "\t\t" << prc[i].BT << "\t\t" << prc[i].WT << "\t\t" << prc[i].CT << endl;
	
	cout << endl << "���μ��� �Ϸ� ���� : ";
	for (int i = 0; i < num; i++)
		cout << "P" << arr2[i] << " ";
	cout << endl << "��� ��ȯ�ð� : " << avgTT << endl << "��� ���ð� : " << avgWT << endl << "��� ����ð� : " << avgRST << endl;
}