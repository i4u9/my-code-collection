let score = JSON.parse(localStorage.getItem('score')) || {
    wins: 0,
    losses: 0,
    ties: 0
};

const moveStrings = {
    rock: '✊',
    paper: '🖐️',
    scissors: '✌️'
};

function playGame(userMove) {
    const randomNumber = Math.random();
    let computerMove = '';
    let result = '';

    if (randomNumber >= 0 && randomNumber < 1/3) {
        computerMove = moveStrings.rock;
    } else if (randomNumber >= 1/3 && randomNumber < 2/3) {
        computerMove = moveStrings.paper;
    } else {
        computerMove = moveStrings.scissors;
    }

    const winCases = userMove === moveStrings.rock && computerMove === moveStrings.scissors || userMove === moveStrings.paper && computerMove === moveStrings.rock || userMove === moveStrings.scissors && computerMove === moveStrings.paper;
    const tieCase = userMove === computerMove;
    if (tieCase) {
        result = 'Tied';
    } else if (winCases) {
        result = 'Won';
    } else {
        result = 'Lost';
    }

    document.querySelector('.resultDisplay').innerText = `You ${result}`;
    document.querySelector('.movesDisplay').innerHTML = `<b>You: ${userMove}   ${computerMove} :Computer</b>`;
    
    return result;
}

function manageScore(result) {
    if (result === 'Won') {
        score.wins++;
    } else if (result === 'Lost') {
        score.losses += 1;
    } else {
        score.ties += 1;
    }

    updateScoreElement();
    localStorage.setItem('score', JSON.stringify(score));
}

function resetScore()
{
    score.losses = 0;
    score.wins = 0; 
    score.ties = 0;
    document.querySelector('.resultDisplay').innerText = '';
    document.querySelector('.movesDisplay').innerText = '';
    updateScoreElement();
    localStorage.setItem('score', JSON.stringify(score));
}

function updateScoreElement() {
    document.getElementById('scoreDisplay').innerText = `Wins: ${score.wins}, Losses: ${score.losses}, Ties: ${score.ties}`;
}

updateScoreElement();