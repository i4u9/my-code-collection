let todoList = [{
    name: 'praise king von',
    dueDate: 'kk'
}];

function addTodo() {
    const inputElement = document.querySelector('.js-todo-input');
    const dateElement = document.querySelector('.js-date-input');
    const todoElement  = document.querySelector('.js-todo-list');

    if (inputElement.value || dateElement.value) {
        const todoObject = {
            name: inputElement.value,
            dueDate: dateElement.value
        }

        todoList.push(todoObject);
        inputElement.value = '';
        dateElement.value = '';

        renderTodoList();
    } else {
        alert('Please, fill all the blanks')
    }
}

function handleAddOnKey(event) {
    if (event.key === 'Enter') {
        addTodo();
    } 
}

function renderTodoList() {
    let todoListHTML = '';

    for (let i = 0; i < todoList.length; i++) {
        const todoObject = todoList[i];
        todoListHTML += `
        <div>${todoObject.name}</div>
        <div>${todoObject.dueDate}</div>
        <button class="delete-button" onclick="deleteTodo(${i});">Delete</button>`;
    }

    document.querySelector('.js-todo-list').innerHTML = todoListHTML;
}

function deleteTodo(todoIndex) {
    todoList.splice(todoIndex, 1);

    renderTodoList();
}

renderTodoList();