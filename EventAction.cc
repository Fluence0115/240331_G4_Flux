#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4THitsMap.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"

#include "EventAction.hh"

EventAction::EventAction()
    // fHCID라는 멤버변수를 -1로 초기화하는 의미
    : G4UserEventAction(), fHCID(-1)
{
}

EventAction::~EventAction()
{
}

void EventAction::BeginOfEventAction(const G4Event *)
{
}

void EventAction::EndOfEventAction(const G4Event *anEvent)
{
    auto HCE = anEvent->GetHCofThisEvent();
    if (!HCE)
        return;

    /* 
    fHCID의 값이 -1일 때만 GetCollectionID 함수를 이용하여 fHCID를 적절한 HC fHCID를 적절한 HC의 ID로 설정하게 되고, 
    이후에는 fHCID에 -1이 아닌 다른 값이 들어있으므로 더이상 if문 내부가 실행되지 않습니다.
    */
    if (fHCID == -1)
        fHCID = G4SDManager::GetSDMpointer()->GetCollectionID("Detector/EDep");
    
    /* 
    HCE->GetHC(fHCID) : HCE(HCofThisEvent)로부터 fHCID라는 ID를 가진 HC를 가져옴
    static_cast<G4THitsMap *>(HCE->GetHC(fHCID)) : 앞서 가져온 HC를 G4THitsMap<G4double> * 자료형으로 형변환함
    auto hitsMap = static_cast<G4THitsMap *>(HCE->GetHC(fHCID)); : 이걸 hitsMap이라는 변수명으로 저장
    */
    auto hitsMap = static_cast<G4THitsMap<G4double> *>(HCE->GetHC(fHCID));
    
    auto analysisManager = G4AnalysisManager::Instance();
    
    /* 
    hitsMap에 있는 사물함 각각을 iter라는 변수로 지정하여 반복문을 수행
    사물함 안에 있는 값(*(iter.second))을 eDep이라는 변수명으로 저장
    만약 eDep의 값이 0보다 크다면 화면에 “— Energy Deposit: “과 “eDep 값을 MeV로 나눈 값"을 이어서 출력하고 줄바꿈
    */
    for (const auto &iter : *(hitsMap->GetMap()))
    {

        // 이번 Event에서 만들어진 HCofThisEvent를 가져와서 HCE라는 변수로 지정. 만약 HCE가 유효하지 않다면 EndOfEventAction() 함수를 종료
        auto eDep = *(iter.second);
        if (eDep > 0.)
        {

            // Fill함수를 사용해서 데이터 누적
            analysisManager->FillH1(0, eDep / MeV);

            analysisManager->FillNtupleDColumn(0, eDep / MeV);
            analysisManager->AddNtupleRow();
        }
    }
}
