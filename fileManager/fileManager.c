#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>

// 매크로
#define MAX_PATH_LEN 200
#define MAX_FILENAME_LEN 100
#define MAX_CONTENT_LEN 1000
#define DATE_FORMAT "%Y-%m-%d %H:%M"
#define PRINT_LINE printf("----------------------------------------------------\n");
#define GET_FILE_PATH(folderPath, fileName, filePath) \
    sprintf(filePath, "%s/%s", folderPath, fileName)
#define PRINT_ERROR(message) \
    perror(message)

// 파일 핸들링
void showFileList(const char *folderPath);
void createFile(const char *folderPath, const char *fileName, const char *content);
void readFile(const char *folderPath, const char *fileName);
void updateFile(const char *folderPath, const char *fileName, const char *newContent);
void deleteFile(const char *folderPath, const char *fileName);

// 디렉토리 관련
void createCategory(const char *folderPath, const char *categoryName);
void deleteCategory(const char *folderPath, const char *categoryName);
void addToCategory(const char *folderPath, const char *itemName, const char *categoryName);

int main() {
    char folderPath[MAX_PATH_LEN];
    
    printf("접근할 폴더 경로를 입력하세요: ");
    scanf("%s", folderPath);
    
    int choice;
    
    do {
        printf("\n-----[MENU]-----\n");
        printf("1. 파일 목록 보기\n");
        printf("2. 파일 생성\n");
        printf("3. 파일 읽기\n");
        printf("4. 파일 수정\n");
        printf("5. 파일 삭제\n");
        printf("6. 카테고리 생성\n");
        printf("7. 카테고리 삭제\n");
        printf("8. 카테고리에 추가\n");
        printf("9. 종료\n");
        printf("-----------------\n");
        printf("원하는 작업을 선택하세요: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                showFileList(folderPath);
                break;
            case 2:
            {
                char fileName[MAX_FILENAME_LEN];
                char content[MAX_CONTENT_LEN];
                
                printf("파일 이름을 입력하세요: ");
                scanf("%s", fileName);
                printf("파일 내용을 입력하세요: ");
                getchar();  // 이전 입력에서의 개행문자 소비
                fgets(content, sizeof(content), stdin);
                
                createFile(folderPath, fileName, content);
            }
                break;
            case 3:
            {
                char fileName[MAX_FILENAME_LEN];
                printf("읽을 파일 이름을 입력하세요: ");
                scanf("%s", fileName);
                readFile(folderPath, fileName);
            }
                break;
            case 4:
            {
                char fileName[MAX_FILENAME_LEN];
                char newContent[MAX_CONTENT_LEN];
                
                printf("수정할 파일 이름을 입력하세요: ");
                scanf("%s", fileName);
                printf("새로운 내용을 입력하세요: ");
                getchar();  // 이전 입력에서의 개행문자 소비
                fgets(newContent, sizeof(newContent), stdin);
                
                updateFile(folderPath, fileName, newContent);
            }
                break;
            case 5:
            {
                char fileName[MAX_FILENAME_LEN];
                printf("삭제할 파일 이름을 입력하세요: ");
                scanf("%s", fileName);
                deleteFile(folderPath, fileName);
            }
                break;
            case 6:
            {
                char categoryName[MAX_FILENAME_LEN];
                printf("생성할 카테고리 이름을 입력하세요: ");
                scanf("%s", categoryName);
                createCategory(folderPath, categoryName);
            }
                break;
            case 7:
            {
                char categoryName[MAX_FILENAME_LEN];
                printf("삭제할 카테고리 이름을 입력하세요: ");
                scanf("%s", categoryName);
                deleteCategory(folderPath, categoryName);
            }
                break;
            case 8:
            {
                char itemName[MAX_FILENAME_LEN];
                char categoryName[MAX_FILENAME_LEN];
                printf("추가할 아이템 이름을 입력하세요: ");
                scanf("%s", itemName);
                printf("추가할 카테고리 이름을 입력하세요: ");
                scanf("%s", categoryName);
                addToCategory(folderPath, itemName, categoryName);
            }
                break;
            case 9:
                printf("프로그램을 종료합니다.\n");
                break;
            default:
                printf("잘못된 선택입니다. 올바른 옵션을 선택하세요.\n");
        }
    } while (choice != 9);
    
    return 0;
}

/* 파일관련 모듈 */
// 파일목록 보기
void showFileList(const char *folderPath) {
    printf("\n\n%-30s %31s\n", "파일 목록", "최근 수정 날짜");
    PRINT_LINE;

    DIR *dir;
    struct dirent *entry;

    dir = opendir(folderPath);

    if (dir == NULL) {
        PRINT_ERROR("폴더 열기 오류");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        char filePath[MAX_PATH_LEN];
        GET_FILE_PATH(folderPath, entry->d_name, filePath);

        struct stat fileStat;
        if (stat(filePath, &fileStat) == -1) {
            PRINT_ERROR("파일 정보 읽기 오류");
            continue;
        }

        char dateStr[20];
        strftime(dateStr, sizeof(dateStr), DATE_FORMAT, localtime(&fileStat.st_mtime));

        printf("%-30s %20s\n", entry->d_name, dateStr);
    }
    
    PRINT_LINE;

    closedir(dir);
}

// 파일 생성
void createFile(const char *folderPath, const char *fileName, const char *content) {
    char filePath[MAX_PATH_LEN];
    GET_FILE_PATH(folderPath, fileName, filePath);

    FILE *file = fopen(filePath, "w");

    if (file == NULL) {
        PRINT_ERROR("파일 생성 오류");
        return;
    }

    fprintf(file, "%s", content);

    fclose(file);

    printf("파일이 성공적으로 생성되었습니다: %s\n", filePath);
}

// 파일 읽기
void readFile(const char *folderPath, const char *fileName) {
    char filePath[MAX_PATH_LEN];
    GET_FILE_PATH(folderPath, fileName, filePath);

    FILE *file = fopen(filePath, "r");

    if (file == NULL) {
        PRINT_ERROR("파일 읽기 오류");
        return;
    }

    char buffer[MAX_CONTENT_LEN];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }

    fclose(file);
}

// 파일 수정
void updateFile(const char *folderPath, const char *fileName, const char *newContent) {
    char filePath[MAX_PATH_LEN];
    GET_FILE_PATH(folderPath, fileName, filePath);

    FILE *file = fopen(filePath, "w");

    if (file == NULL) {
        PRINT_ERROR("파일 수정 오류");
        return;
    }

    fprintf(file, "%s", newContent);

    fclose(file);

    printf("파일이 성공적으로 수정되었습니다: %s\n", filePath);
}

// 파일 삭제
void deleteFile(const char *folderPath, const char *fileName) {
    char filePath[MAX_PATH_LEN];
    GET_FILE_PATH(folderPath, fileName, filePath);

    int result = remove(filePath);

    if (result == 0) {
        printf("파일이 성공적으로 삭제되었습니다: %s\n", filePath);
    } else {
        PRINT_ERROR("파일 삭제 오류");
    }
}

/* 카테고리 관련 모듈 */
// 디렉토리 생성
void createCategory(const char *folderPath, const char *categoryName) {
    char categoryPath[MAX_PATH_LEN];
    GET_FILE_PATH(folderPath, categoryName, categoryPath);

    int result = mkdir(categoryPath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    if (result == 0) {
        printf("카테고리가 성공적으로 생성되었습니다: %s\n", categoryPath);
    } else {
        PRINT_ERROR("카테고리 생성 오류");
    }
}

// 디렉토리 삭제
void deleteCategory(const char *folderPath, const char *categoryName) {
    char categoryPath[MAX_PATH_LEN];
    GET_FILE_PATH(folderPath, categoryName, categoryPath);

    int result = rmdir(categoryPath);

    if (result == 0) {
        printf("카테고리가 성공적으로 삭제되었습니다: %s\n", categoryPath);
    } else {
        PRINT_ERROR("카테고리 삭제 오류");
    }
}

// 디렉토리에 추가
void addToCategory(const char *folderPath, const char *itemName, const char *categoryName) {
    char itemPath[MAX_PATH_LEN];
    GET_FILE_PATH(folderPath, itemName, itemPath);

    char categoryPath[MAX_PATH_LEN];
    GET_FILE_PATH(folderPath, categoryName, categoryPath);

    int result = rename(itemPath, categoryPath);

    if (result == 0) {
        printf("%s가 성공적으로 %s 카테고리에 추가되었습니다.\n", itemName, categoryName);
    } else {
        PRINT_ERROR("카테고리에 추가 오류");
    }
}
