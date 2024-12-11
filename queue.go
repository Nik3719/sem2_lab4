package main

import (
	"encoding/binary"
	"encoding/json"
	"fmt"
	"os"
)

type Node struct {
	value string
	next  *Node
}

type Queue struct {
	head *Node
	tail *Node
	size int
}

func (q *Queue) QGET(index int) string {
	if index < 0 || index >= q.size {
		return ""
	}
	current := q.head
	for i := 0; i < index; i++ {
		current = current.next
	}
	return current.value
}

func (q *Queue) Enqueue(value string) {
	newNode := &Node{value: value, next: nil}
	if q.tail != nil {
		q.tail.next = newNode
	} else {
		q.head = newNode
	}
	q.tail = newNode
	q.size++
}

func (q *Queue) Dequeue() string {
	if q.head == nil {
		return "Queue is empty"
	}
	value := q.head.value
	q.head = q.head.next
	if q.head == nil {
		q.tail = nil
	}
	q.size--
	return value
}

func (q *Queue) IsEmpty() bool {
	return q.head == nil
}

func (q *Queue) QLEN() int {
	return q.size
}

func (q *Queue) PRINT() {
	if q.head == nil {
		fmt.Println("Queue is empty")
		return
	}

	current := q.head
	fmt.Print("Queue: ")
	for current != nil {
		fmt.Print(current.value, " ")
		current = current.next
	}
	fmt.Println()
}

// SerializeBin записывает очередь в бинарный файл
func SerializeBin(queue *Queue, filename string) {
	file, _ := os.Create(filename)
	defer file.Close()

	// Записать количество элементов в очереди
	length := queue.QLEN()
	binary.Write(file, binary.LittleEndian, int32(length))

	// Записать элементы очереди
	for i := 0; i < length; i++ {
		str := queue.QGET(i)
		strLen := int32(len(str))
		binary.Write(file, binary.LittleEndian, strLen)
		file.WriteString(str)
	}
}

// DeserializeBin загружает очередь из бинарного файла
func DeserializeBin(queue *Queue, filename string) {
	file, _ := os.Open(filename)
	defer file.Close()

	// Прочитать количество элементов
	var len int32
	binary.Read(file, binary.LittleEndian, &len)

	// Создать очередь из элементов
	for i := int32(0); i < len; i++ {
		var strLen int32
		binary.Read(file, binary.LittleEndian, &strLen)
		str := make([]byte, strLen)
		file.Read(str)
		queue.Enqueue(string(str)) // Добавить строку в очередь
	}
}

func (q *Queue) SerializeToJSON(filename, queueName string) error {
	// Открываем или создаем файл
	file, err := os.OpenFile(filename, os.O_RDWR|os.O_CREATE, 0666)
	if err != nil {
		return err
	}
	defer file.Close()

	// Проверяем размер файла, если файл пустой, записываем пустой объект
	fileInfo, err := file.Stat()
	if err != nil {
		return err
	}

	if fileInfo.Size() == 0 {
		// Если файл пустой, записываем пустой объект {}
		file.Seek(0, 0) // Позиционируем курсор в начало файла
		emptyObject := map[string]interface{}{}
		encoder := json.NewEncoder(file)
		encoder.SetIndent("", "  ")
		return encoder.Encode(emptyObject)
	}

	// Чтение текущего содержимого файла
	var data map[string][]string
	decoder := json.NewDecoder(file)
	// Если файл не пустой, пытаемся его декодировать в map
	if err := decoder.Decode(&data); err != nil && err.Error() != "EOF" {
		if err.Error() == "EOF" {
			data = make(map[string][]string) // Если файл пуст, инициализируем пустую карту
		} else {
			return err
		}
	}

	// Создаем срез значений из очереди
	var queueValues []string
	current := q.head
	for current != nil {
		queueValues = append(queueValues, current.value)
		current = current.next
	}

	// Добавляем или обновляем данные очереди
	data[queueName] = queueValues

	// Перезаписываем файл с новыми данными
	file.Seek(0, 0) // Позиционируем курсор в начало файла
	encoder := json.NewEncoder(file)
	encoder.SetIndent("", "  ") // Форматированный вывод
	return encoder.Encode(data)
}

func (q *Queue) DeserializeQueue(filename, queueName string) error {

	// Открываем файл
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	// Чтение текущего содержимого файла
	var data map[string][]string
	decoder := json.NewDecoder(file)
	if err := decoder.Decode(&data); err != nil {
		return err
	}

	// Проверяем, существует ли очередь с указанным именем
	if queueValues, exists := data[queueName]; exists {
		// Загружаем элементы в очередь
		for _, value := range queueValues {
			q.Enqueue(value)
		}
		return nil
	}
	return fmt.Errorf("queue '%s' not found", queueName)
}