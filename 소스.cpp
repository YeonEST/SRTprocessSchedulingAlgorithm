#include<iostream>
using namespace std;
/*SRTF 알고리즘은 큐에 새로 도착 한 프로세스를 포함한 모든 프로세스 중 가장 짧은 시간을 요구하는
프로그램을 먼저 수행하며 수행 중에서도 비교연산이 매번 일어나 수행중인 프로세스보다 짧은 시간을
요구하는 프로세스가 입력 될 시에 선점된다.*/

#define IM 5 // 임계값으로 선점시 if문으로 이 값보다 낮은 remain time을 가지면 선점 당하지 않게 설정

struct process {/*프로세스 괄리를 위해 Id와 각종 시간 정보를 담은 구조체를 생성
				각 시간들은 강의에서 교수님께서 알려주신 시간 그래프에 나온 시간들을 사용함*/
	int ID; // 프로세스 ID
	int AT; // Arrival time 선점 테스트 및 TRT계산을 위한 도착시간
	int ST; // Start time
	int BT; // Bust time
	int RT; // Remain time 남은 시간으로 BT - 실행시간 값이며 0이 되면 프로그램 완료
	int CT; // Completion time 완료 시간
	int WT; // Waiting time 큐에서 대기한 시간
	int RST; // Respoon time => 응답시간 시작부터 첫 출력까지 걸린 시간, 혹은 실행까지 걸린시간으로 ST - AT
	int TT; // Turnaround time => 대기시간 + 버스트타임 or CT - AT
};

int main() {
	int num; // 프로세스 개수를 입력받을 변수,
	int currentTime = 0; // 이 프로그램(알고리즘)의 시간 카운트
	int totalTT, totalWT, totalRST;// 여기부터 float avg~까지는 평균 대기시간, 반환시간, 응답시간을 구하기 위한 변수 선언
	totalTT = 0; totalWT = 0; totalRST = 0;
	float avgTT, avgWT, avgRST; avgTT = 0; avgWT = 0; avgRST = 0;
	struct process prc[200]; // 최대 프로세스 개수는 200개로 설정
	bool complete_flag[200] = { true, }; // true로 셋 해두고 입력받을 개수만큼 0~(n-1)까지 false로 바꿈

	/*프로세스 정보 입력 시작*/
	cout << "프로세스 개수 : ";
	cin >> num;
	cout << endl;
	for (int i = 0; i < num; i++) {
		prc[i].ID = i; // 프로세스 아이디 배정
		complete_flag[i] = false;
		cout << "프로세스 " << prc[i].ID << "의 도착시간 : ";
		cin >> prc[i].AT;
		cout << "프로세스 " << prc[i].ID << "의 버스트시간 : ";
		cin >> prc[i].BT;
		prc[i].RT = prc[i].BT; // 남은 버스트타임은 아직 실행 안 되었으니 요구 버스트타임과 같음
		cout << endl;
	}
	/*프로세스 정보 입력 종료*/
	bool temp = 0; // 반복 검사를 위한 더미
	while (1) {
		int runID = -1; // 실행할 프로세스의 아이디, 초기값을 -1로 잡음
		/*반복 여부 검사 시작*/
		for (int i = 0; i < num; i++) {
			if (complete_flag[i] == false) {
				temp = 0; // 1개라도 완료가 안 된 프로세스가 있으면 temp를 0로 셋
				break;
			}
			else temp = 1; // 모두 완료되었으면 temp를 1로 셋
		}
		if (temp == 1) break; //  temp 2이면 모두 완료되었음으로 반복문이 종료되게 함
		/*반복 여부 검사 종료*/
		/*스케줄링 시작*/
		int m = 9999999; // 남은 시간 비교에 사용하기 위한 현재 체크중인 프로세스의 남은 버스트타임 기록을 위한 값으로, 초기 값을 크게 설정해둠
		for (int i = 0; i < num; i++) {
			if (prc[runID].RT <= IM && runID != -1) // 첫 실행시에는 runID값이 -1이기 때문에 오류가 발생하여 오류가 발생하지 않게 if조건에 runID != -1를 추가
				break; // 현재 실행중인 프로세스의 남은 버스트타임이 임게값(IM = 5) 이하면 선점당하지 않게 함
			if (prc[i].AT <= currentTime && complete_flag[i] == 0) { // 프로그램 도착시간이 현재시간보다 빨리도착 했거나 같은시간에 도착하고 완료가 안 되었으면 검사
				if (prc[i].RT < m) { // 현재 선정 되어있는 프로세스의 남은 버스트타임보다 더 적게 남은(빨리 끝나는) 프로세스 발견시 실행 할 프로세스를 해당 프로세스로 변경
					m = prc[i].RT;
					runID = i;
				}
				if (prc[i].RT == m) {
					if (prc[i].AT < prc[runID].AT) { // 남은 버스트타임이 같은 프로세스가 존재하는 경우 더 빨리 도착한 프로세스를 선정
						m = prc[i].RT;
						runID = i;
					}
				}
			}
		}

		if (runID != -1) { // 실행 될 프로세스 선정시 실행
			if (prc[runID].RT == prc[runID].BT)
				prc[runID].ST = currentTime; // 한 번도 실행된 적 없는 프로세스면 첫 실행이니 시작시간 = 현재시간으로 기록

			prc[runID].RT -= 1; // 1 만큼의 시간동안 실행되었음으로 남은시간이 -1
			currentTime++; // 현재시간 1 증가

			if (prc[runID].RT == 0) { // 프로세스 실행 완료시 완료시간, 턴어라운드시간, 대기시간, 응답시간 등을 계산하여 입력
				/*프로세스 관련 정보 입력*/
				prc[runID].CT = currentTime; // 완료시각을 현재시간으로 설정
				prc[runID].TT = prc[runID].CT - prc[runID].AT; // 턴어라운드타임은 완료시간 - 도착시간
				prc[runID].WT = prc[runID].TT - prc[runID].BT; // 대기시간은 실행시간 - 도착시간
														//혹은 턴어라운드타임 = 대기시간+실행시간에서 대기시간 = 턴어라운드 - 실행시간으로 구함 여기선 후자
				prc[runID].RST = prc[runID].ST - prc[runID].AT; // 응답시간 = 시작시간 - 도착시간
				/*평균값을 위한 토탈 값 입력*/
				totalTT += prc[runID].TT;
				totalWT += prc[runID].WT;
				totalRST += prc[runID].RST;

				complete_flag[runID] = true; // 해당 ID의 프로세스를 완료되었다 기록
			}
		}
		else break; // 중앙처리장치를 점유할 프로세스가 없으면 종료
	}
	/*스케줄링 종료*/
	/*스케줄링 이후 평균값 계산 시작*/
	avgTT = (float)totalTT / num; avgWT = (float)totalWT / num; avgRST = (float)totalRST / num; // total자료들이 int형이기 때문에 캐스트 연산자로 float화 시켜준다

	int arr1[200], arr2[200]; // 프로세스를 완료 순서대로 정렬하기 위해 프로세스 인덱스를 넣을 배열 생성, arr1은 완료시간 arr2는 ID
	for (int i = 0; i < num; i++) arr1[i] = prc[i].CT; // arr1에 프로세스 ID 별 완료시간 입력
	for (int i = 0; i < num; i++) arr2[i] = prc[i].ID; // arr2에 프로세스 ID 입력
	for (int i = 0; i < num-1; i++) { // 완료시간별로 정렬, 알고리즘은 선택정렬을 이용하였음
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
	} // 정렬 끝
	cout << "프로세스 정보" << endl << "프로세스 ID\t도착시간\t버스트타임\t대기시간\t완료시간" << endl;
	for (int i = 0; i < num; i++)
		cout << prc[i].ID << "\t\t" << prc[i].AT << "\t\t" << prc[i].BT << "\t\t" << prc[i].WT << "\t\t" << prc[i].CT << endl;
	
	cout << endl << "프로세스 완료 순서 : ";
	for (int i = 0; i < num; i++)
		cout << "P" << arr2[i] << " ";
	cout << endl << "평균 반환시간 : " << avgTT << endl << "평균 대기시간 : " << avgWT << endl << "평균 응답시간 : " << avgRST << endl;
}