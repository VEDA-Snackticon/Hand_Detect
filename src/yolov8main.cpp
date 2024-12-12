#include "yoloV8.h"
#include "BYTETracker.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <set>
#include <cstdint>

#include <unordered_map>

//임시
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

// 키 입력 상태를 비차단으로 설정하는 함수
void enableNonBlockingInput() {
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);       // 현재 터미널 설정 읽기
    t.c_lflag &= ~(ICANON | ECHO);    // 비캐논 모드와 에코 제거
    tcsetattr(STDIN_FILENO, TCSANOW, &t); // 변경 적용

    // 파일 디스크립터를 비차단 모드로 설정
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
}

// 키 입력 상태를 원래대로 복원하는 함수
void disableNonBlockingInput() {
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag |= (ICANON | ECHO);     // 캐논 모드와 에코 복원
    tcsetattr(STDIN_FILENO, TCSANOW, &t);

    // 파일 디스크립터를 차단 모드로 복원
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK);
}

static int target_size = 640;
static int framerate = 30;
static YoloV8* yolov8 = new YoloV8(target_size);
static BYTETracker* tracker = new BYTETracker(framerate, 15); //조정 필요
// static std::vector<Object> objects;
// static std::vector<Object> filtered_objects;
// static std::vector<STrack> output_stracks;



    /* for track IDs */
static std::set<int> current_ids; // 현재 프레임에서 추적 중인 ID
static std::set<int> previous_ids; // 이전 프레임에서 추적 중인 ID
static std::set<int> completed_tracks; // 들어왔다 나간 손의 ID를 저장
static int CNT = 0;


/* hand check */
bool check_hand_exit(const std::set<int>& previous_ids, const std::set<int>& current_ids, std::set<int>& completed_tracks)
{
    for (int id : previous_ids) {
        if (current_ids.find(id) == current_ids.end()){// && completed_tracks.find(id) == completed_tracks.end()) {
            // completed_tracks.insert(id);
            std::cout<< "Hand exited: ID " << id << std::endl; //디버그 메시지
            return true; // 손이 들어왔다 나간 것을 감지
        }
    }
    return false;
}

extern "C" 
{
    int event_main(uint8_t* buffer, size_t size, time_t timestamp, int width, int height)
    {
        enableNonBlockingInput();

        char key;
        // bool running = true;

        // while (running) {
            // 키 입력 확인
            key = getchar();
            if (key == 'q') { // 'q' 키가 입력되면
                // running = false;
                std::cout << "Hand entered and exited the frame." << std::endl;
                disableNonBlockingInput(); // 비차단 입력 비활성화
                return 1;                  // 프로그램 종료
            }

            // 다른 동작 수행 (예: 출력 작업)
            // std::cout << "Program is running. Press 'q' to quit.\r";
            // std::cout.flush();

            // usleep(100000); // 0.1초 대기
        // }

        // 비차단 입력 비활성화
        disableNonBlockingInput();
        
        /*
        // CNT++;
        if(buffer == nullptr){
            return -1;
        }

        cv::Mat local_buffer(height, width, CV_8UC2, buffer);
        // cv::Mat frame = cv::imdecode(local_buffer, cv::IMREAD_COLOR);
        cv::Mat frame;

        cv::cvtColor(local_buffer, frame, cv::COLOR_YUV2BGR_YUNV);

        std::string fname;

        // fname.append(std::to_string(timestamp));
        // fname.append(".jpg");
        // cv::imwrite(fname, frame);
        if (frame.empty())
        {
            std::cerr << "Error: Failed to grab frame.\n";
            return -1;
        }
        std::vector<Object> objects;

        const int target_label = 1;
        int rtn = 0;



        yolov8->detect(frame, objects);
        
        // std::cout << "object iter start" <<std::endl;

        std::vector<Object> filtered_objects;
        for (const auto& obj : objects) {
            // std::cout << "label : " << obj.label << std::endl; //label check
            if (obj.label == target_label) { // hand
                filtered_objects.push_back(obj);
            } 
        }
        // std::cout << "object iter end" <<std::endl;
        
        // object tracking
        output_stracks = tracker->update(filtered_objects);

        // ID update

        current_ids.clear();

        for(const auto& track : output_stracks) {
            current_ids.insert(track.track_id);
            // std::cout<<"insert good?" <<std::endl;
        }


        if((CNT % 100) == 0){
            
            std::cout<<"Event ocurred"<<std::endl;
            return 1;
        }


        // debug
        // std::cout << "Previous IDs: ";
        // for (int id : previous_ids) std::cout << id << " ";
        // std::cout << std::endl;

        // std::cout << "Current IDs: ";
        // for (int id : current_ids) std::cout << id << " ";
        // std::cout << std::endl;

        // std::cout << "Completed tracks: ";
        // for (int id : completed_tracks) std::cout << id << " ";
        // std::cout << std::endl;


        if (check_hand_exit(previous_ids, current_ids, completed_tracks)) {
            std::cout << "Hand entered and exited the frame." << std::endl;
            rtn = 1; // 손이 들어왔다 나간 경우 1을 반환

            // fname.append(std::to_string(timestamp));
            // fname.append(".jpg");
            // cv::imwrite(fname, frame);
        }
        else {
            rtn = 0;
        }
        previous_ids = current_ids;

        return rtn;
        */
    }
}
