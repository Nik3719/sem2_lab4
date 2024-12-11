package main

import (
	"encoding/json"
	"os"
	"testing"
)

func TestEnqueue(t *testing.T) {
	// Создаем очередь
	queue := &Queue{}

	// Добавляем элементы в очередь
	queue.Enqueue("first")
	queue.Enqueue("second")

	// Проверяем размер очереди
	if queue.QLEN() != 2 {
		t.Errorf("Expected QLEN 2, got %d", queue.QLEN())
	}

	// Проверяем значения в очереди
	if queue.head.value != "first" {
		t.Errorf("Expected head value 'first', got '%s'", queue.head.value)
	}
	if queue.tail.value != "second" {
		t.Errorf("Expected tail value 'second', got '%s'", queue.tail.value)
	}
}

func TestDequeue(t *testing.T) {
	// Создаем очередь и добавляем элементы
	queue := &Queue{}
	queue.Enqueue("first")
	queue.Enqueue("second")

	// Делаем dequeue и проверяем значение
	value := queue.Dequeue()
	if value != "first" {
		t.Errorf("Expected dequeued value 'first', got '%s'", value)
	}

	// Проверяем размер очереди после dequeue
	if queue.QLEN() != 1 {
		t.Errorf("Expected QLEN 1, got %d", queue.QLEN())
	}

	// Делаем еще один dequeue
	value = queue.Dequeue()
	if value != "second" {
		t.Errorf("Expected dequeued value 'second', got '%s'", value)
	}

	// Проверяем, что очередь теперь пуста
	if !queue.IsEmpty() {
		t.Errorf("Expected queue to be empty")
	}
}

func TestDequeueEmptyQueue(t *testing.T) {
	// Создаем пустую очередь
	queue := &Queue{}

	// Пытаемся сделать dequeue на пустой очереди
	value := queue.Dequeue()
	if value != "Queue is empty" {
		t.Errorf("Expected 'Queue is empty', got '%s'", value)
	}
}

func TestIsEmpty(t *testing.T) {
	// Создаем очередь
	queue := &Queue{}

	// Проверяем, что очередь пуста
	if !queue.IsEmpty() {
		t.Errorf("Expected queue to be empty")
	}

	// Добавляем элемент в очередь
	queue.Enqueue("first")

	// Проверяем, что очередь теперь не пуста
	if queue.IsEmpty() {
		t.Errorf("Expected queue to be not empty")
	}
}

func TestQLEN(t *testing.T) {
	// Создаем очередь
	queue := &Queue{}

	// Проверяем размер пустой очереди
	if queue.QLEN() != 0 {
		t.Errorf("Expected QLEN 0, got %d", queue.QLEN())
	}

	// Добавляем элементы
	queue.Enqueue("first")
	queue.Enqueue("second")

	// Проверяем размер после добавления элементов
	if queue.QLEN() != 2 {
		t.Errorf("Expected QLEN 2, got %d", queue.QLEN())
	}

	// Убираем один элемент
	queue.Dequeue()

	// Проверяем размер после удаления
	if queue.QLEN() != 1 {
		t.Errorf("Expected QLEN 1, got %d", queue.QLEN())
	}
}

func TestSerializeDeserialize(t *testing.T) {
	filename := "test_queue.bin"

	// Создать и заполнить очередь
	originalQueue := &Queue{}
	originalQueue.Enqueue("A")
	originalQueue.Enqueue("B")
	originalQueue.Enqueue("C")

	// Сериализовать очередь в файл
	SerializeBin(originalQueue, filename)

	defer os.Remove(filename) // Удалить файл после теста

	// Создать новую пустую очередь
	deserializedQueue := &Queue{}

	// Десериализовать из файла
	DeserializeBin(deserializedQueue, filename)

	// Проверить длину очереди
	if deserializedQueue.QLEN() != originalQueue.QLEN() {
		t.Errorf("Ожидалась длина %d, получена %d", originalQueue.QLEN(), deserializedQueue.QLEN())
	}

	// Проверить элементы очереди
	for i := 0; i < originalQueue.QLEN(); i++ {
		originalValue := originalQueue.QGET(i)
		deserializedValue := deserializedQueue.QGET(i)
		if originalValue != deserializedValue {
			t.Errorf("На позиции %d ожидалось %q, получено %q", i, originalValue, deserializedValue)
		}
	}
}

func TestQueueOperations(t *testing.T) {
	queue := &Queue{}

	// Проверка пустой очереди
	if !queue.IsEmpty() {
		t.Errorf("Ожидалось, что очередь будет пустой")
	}

	// Добавление элементов
	queue.Enqueue("first")
	queue.Enqueue("second")

	if queue.IsEmpty() {
		t.Errorf("Ожидалось, что очередь не будет пустой")
	}

	// Проверка длины
	if queue.QLEN() != 2 {
		t.Errorf("Ожидалась длина 2, получена %d", queue.QLEN())
	}

	// Удаление элементов
	val := queue.Dequeue()
	if val != "first" {
		t.Errorf("Ожидалось значение 'first', получено '%s'", val)
	}

	val = queue.Dequeue()
	if val != "second" {
		t.Errorf("Ожидалось значение 'second', получено '%s'", val)
	}

	// Проверка на пустую очередь после удаления
	if !queue.IsEmpty() {
		t.Errorf("Ожидалось, что очередь будет пустой после удаления всех элементов")
	}
}

func TestQGETError(t *testing.T) {
	queue := &Queue{}
	val := queue.QGET(1)
	if val != "" {
		t.Errorf("Ожидалось сообщение 'Index out of range', получено '%s'", val)
	}
}

func TestQueuePrint(t *testing.T) {
	// Create a queue and populate it with some values
	q := &Queue{}

	q.PRINT()

	q.Enqueue("1")
	q.Enqueue("2")
	q.Enqueue("3")

	q.PRINT() // This should now write to `buf`
}


func TestQueue_SerializeToJSON(t *testing.T) {
	// Создаем тестовую очередь
	q := &Queue{}
	q.Enqueue("item1")
	q.Enqueue("item2")

	// Имя файла для теста
	filename := "test_queues.json"
	queueName := "Queue1"

	// Очищаем файл перед тестом
	os.Remove(filename)

	// Сериализуем очередь в JSON файл
	err := q.SerializeToJSON(filename, queueName)
	if err != nil {
		t.Errorf("expected no error, got %v", err)
	}

	// Проверяем, что файл был создан
	if _, err := os.Stat(filename); os.IsNotExist(err) {
		t.Errorf("expected file %s to be created", filename)
	}

	// Открываем файл и проверяем его содержимое
	file, err := os.Open(filename)
	if err != nil {
		t.Fatalf("failed to open file: %v", err)
	}
	defer file.Close()

	// Читаем содержимое файла и проверяем, что очередь была сериализована корректно
	var data map[string][]string
	decoder := json.NewDecoder(file)
	if err := decoder.Decode(&data); err != nil {
		t.Fatalf("failed to decode JSON: %v", err)
	}
}

func TestQueue_DeserializeQueue(t *testing.T) {
	// Создаем новый объект Queue
	q := &Queue{}

	// Имя файла и очереди
	filename := "test_queues.json"
	queueName := "Queue1"

	// Добавляем тестовые данные в файл
	q.Enqueue("item1")
	q.Enqueue("item2")
	err := q.SerializeToJSON(filename, queueName)
	if err != nil {
		t.Errorf("expected no error, got %v", err)
	}

	// Создаем новый объект Queue для десериализации
	q2 := &Queue{}

	// Десериализуем очередь из файла
	err = q2.DeserializeQueue(filename, queueName)
	if err != nil {
		t.Errorf("expected no error, got %v", err)
	}

	// Проверяем, что элементы десериализованы корректно
	if q2.head == nil || q2.head.value != "item1" {
		t.Errorf("expected first item to be 'item1', got %v", q2.head.value)
	}
	if q2.head != nil && q2.head.next == nil || q2.head.next.value != "item2" {
		t.Errorf("expected second item to be 'item2', got %v", q2.head.next.value)
	}
}

func TestQueue_DeserializeQueue_NotFound(t *testing.T) {
	// Создаем новый объект Queue
	q := &Queue{}

	// Имя файла и очереди, которая не существует
	filename := "test_queues.json"
	queueName := "NonExistentQueue"

	// Попытка десериализации очереди, которой нет в файле
	err := q.DeserializeQueue(filename, queueName)
	if err == nil {
		t.Errorf("expected error, got nil")
	} else if err.Error() != "queue 'NonExistentQueue' not found" {
		t.Errorf("expected error 'queue \"NonExistentQueue\" not found', got %v", err)
	}
}

func TestQueue_SerializeToJSON_EmptyFile(t *testing.T) {
	// Создаем новый объект Queue
	q := &Queue{}
	queueName := "Queue1"
	filename := "empty_test_queues.json"

	// Очищаем файл перед тестом
	os.Remove(filename)

	// Сериализуем пустую очередь в файл
	err := q.SerializeToJSON(filename, queueName)
	if err != nil {
		t.Errorf("expected no error, got %v", err)
	}

	// Открываем файл и проверяем его содержимое
	file, err := os.Open(filename)
	if err != nil {
		t.Fatalf("failed to open file: %v", err)
	}
	defer file.Close()

	// Читаем содержимое файла и проверяем, что он не пустой
	var data map[string][]string
	decoder := json.NewDecoder(file)
	if err := decoder.Decode(&data); err != nil {
		t.Fatalf("failed to decode JSON: %v", err)
	}
}
