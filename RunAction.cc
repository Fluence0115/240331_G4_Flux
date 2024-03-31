#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "RunAction.hh"
#include "G4SystemOfUnits.hh"

// #include "g4csv.hh"

// 생성자
RunAction::RunAction()
    : G4UserRunAction()
{   
    // analysisManager라는 변수명으로 G4AnalysisManager를 이용할 수 있게 하는 명령어
    auto analysisManager = G4AnalysisManager::Instance();
    
    // CreateH1함수 사용해서 히스토그램 완성-> 0~3까지 1024개로 나눠 히스토그램을 완성함
    analysisManager->CreateH1("EDep", "Energy Deposition in the box", 1024, 0., 3.);
    
    // CreatNtuple함수를 사용하여 Energy Deposition이라고 지은 Ntuple을 만듦
    analysisManager->CreateNtuple("EDep", "Energy Deposition in the box");

    // Energy Deposition 값에 해당하는 한 열만 기록. 실수값이므로 Double(D)형 사용
    analysisManager->CreateNtupleDColumn("EDep");
    
    // FinishNtuple 함수를 사용하여 완성
    analysisManager->FinishNtuple();
}

// 소멸자
RunAction::~RunAction()
{
    delete G4AnalysisManager::Instance();
}

void RunAction::BeginOfRunAction(const G4Run *)
{
   
    auto analysisManager = G4AnalysisManager::Instance();
    
    // G4AnalysisManager 클래스가 제공하는 OpenFile 함수사용 : 출력파일 열기->이름
    analysisManager->OpenFile("G4_Minimal.csv");
}

void RunAction::EndOfRunAction(const G4Run *)
{

    auto analysisManager = G4AnalysisManager::Instance();
    
    // 출력파일을 닫기 전에 Write함수를 사용하여 데이터의 기록
    analysisManager->Write();

    // G4AnalysisManager 클래스가 제공하는 CloseFile 함수사용 : 출력파일 닫기
    analysisManager->CloseFile();
}
