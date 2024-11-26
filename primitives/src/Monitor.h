class Monitor {
private:
    int resource; // Общий ресурс
    std::mutex mtx; // Мьютекс для синхронизации
    std::condition_variable cv; // Условная переменная
    bool ready = true; // Флаг готовности ресурса

public:
    Monitor() : resource(0) {}

    void produce(int value) {
        std::unique_lock<std::mutex> lock(mtx);
        resource = value; // Изменяем ресурс
        ready = true; // Устанавливаем флаг готовности
        cv.notify_one(); // Уведомляем ожидающий поток
    }

    void consume() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return ready; }); // Ожидаем, пока ресурс не будет готов
        // std::cout << "Consumed: " << resource << std::endl;
        ready = false; // Сбрасываем флаг готовности
    }
};
