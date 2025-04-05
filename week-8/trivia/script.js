document.addEventListener('DOMContentLoaded', function() {
    // multiple choice question handling
    const choices = document.querySelectorAll('.choice');
    const mcFeedback = document.getElementById('mc-feedback');

    choices.forEach(choice => {
        choice.addEventListener('click', function() {
            // remove any previous feedback
            mcFeedback.textContent = '';
            mcFeedback.className = 'feedback';

            // remove any previous styling from all buttons
            choices.forEach(btn => {
                btn.classList.remove('correct', 'incorrect');
            });

            // check if the answer is correct
            if (this.dataset.correct === 'true') {
                this.classList.add('correct');
                mcFeedback.textContent = 'Correct!';
                mcFeedback.classList.add('correct');
            } else {
                this.classList.add('incorrect');
                mcFeedback.textContent = 'Incorrect';
                mcFeedback.classList.add('incorrect');
            }
        });
    });

    // free response question handling
    const freeResponse = document.getElementById('free-response');
    const submitButton = document.getElementById('submit-answer');
    const frFeedback = document.getElementById('fr-feedback');

    submitButton.addEventListener('click', function() {
        // remove any previous feedback
        frFeedback.textContent = '';
        frFeedback.className = 'feedback';
        freeResponse.classList.remove('correct', 'incorrect');

        // get the user's answer and convert to lowercase for comparison
        const userAnswer = freeResponse.value.trim().toLowerCase();
        const correctAnswer = 'jupiter';

        if (userAnswer === correctAnswer) {
            freeResponse.classList.add('correct');
            frFeedback.textContent = 'Correct!';
            frFeedback.classList.add('correct');
        } else {
            freeResponse.classList.add('incorrect');
            frFeedback.textContent = 'Incorrect';
            frFeedback.classList.add('incorrect');
        }
    });

    // allow submission with enter key
    freeResponse.addEventListener('keypress', function(e) {
        if (e.key === 'Enter') {
            submitButton.click();
        }
    });
}); 