# Название вашего модуля
MODULE_NAME = mymodule

# Имя файла отчета о покрытии
COVERAGE_FILE = coverage.out
COVERAGE_HTML = coverage.html

# Цель по умолчанию: тестирование
.PHONY: all
all: test

# Цель для тестирования с отчетом о покрытии
.PHONY: test
test:
	@echo "Running tests..."
	go test -coverprofile=$(COVERAGE_FILE)

# Генерация отчета в формате HTML
.PHONY: coverage
coverage: test
	@echo "Generating coverage report..."
	go tool cover -html=$(COVERAGE_FILE) -o $(COVERAGE_HTML)

# Цель для очистки временных файлов
.PHONY: clean
clean:
	@echo "Cleaning up..."
	rm -f $(COVERAGE_FILE) $(COVERAGE_HTML)

# Цель для запуска программы
.PHONY: run
run:
	@echo "Running the application..."
	go run main.go queue.go
