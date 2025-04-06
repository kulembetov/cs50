// animation timing function
const ANIMATION_DURATION = 300;

// show edit form with animation
function showEditForm(id, name, month, day) {
    const modal = document.getElementById('editModal');
    const modalContent = modal.querySelector('.modal-content');
    
    // set form values
    document.getElementById('editId').value = id;
    document.getElementById('editName').value = name;
    document.getElementById('editMonth').value = month;
    document.getElementById('editDay').value = day;
    
    // show modal with animation
    modal.style.display = 'flex';
    setTimeout(() => {
        modal.classList.add('show');
        modalContent.style.transform = 'translateY(0)';
    }, 10);
}

// hide edit form with animation
function hideEditForm() {
    const modal = document.getElementById('editModal');
    const modalContent = modal.querySelector('.modal-content');
    
    // hide modal with animation
    modal.classList.remove('show');
    modalContent.style.transform = 'translateY(20px)';
    
    // remove focus from active element
    document.activeElement.blur();
    
    setTimeout(() => {
        modal.style.display = 'none';
    }, ANIMATION_DURATION);
}

// delete birthday with confirmation
function deleteBirthday(id) {
    if (confirm('Are you sure you want to delete this birthday?')) {
        const form = document.createElement('form');
        form.method = 'POST';
        form.action = '/delete';
        
        const input = document.createElement('input');
        input.type = 'hidden';
        input.name = 'id';
        input.value = id;
        
        form.appendChild(input);
        document.body.appendChild(form);
        form.submit();
    }
}

// handle edit form submission
document.getElementById('editForm').addEventListener('submit', function(e) {
    e.preventDefault();
    
    const form = this;
    const submitButton = form.querySelector('button[type="submit"]');
    
    // add loading state
    submitButton.classList.add('loading');
    submitButton.disabled = true;
    
    // create form data
    const formData = new FormData(form);
    
    // submit form
    fetch('/edit', {
        method: 'POST',
        body: formData
    })
    .then(response => {
        if (response.ok) {
            window.location.reload();
        } else {
            throw new Error('Failed to update birthday');
        }
    })
    .catch(error => {
        alert(error.message);
        submitButton.classList.remove('loading');
        submitButton.disabled = false;
    });
});

// close modal when clicking outside
document.getElementById('editModal').addEventListener('click', function(e) {
    if (e.target === this) {
        hideEditForm();
    }
});

// close modal with escape key
document.addEventListener('keydown', function(e) {
    if (e.key === 'Escape' && document.getElementById('editModal').style.display === 'flex') {
        hideEditForm();
    }
});

// initialize animations
document.addEventListener('DOMContentLoaded', function() {
    // set current year in footer
    document.getElementById('year').textContent = new Date().getFullYear();

    // animate sections
    const sections = document.querySelectorAll('.section');
    sections.forEach((section, index) => {
        setTimeout(() => {
            section.classList.add('show');
        }, index * 100);
    });
    
    // animate table rows
    const rows = document.querySelectorAll('.table-row');
    rows.forEach((row, index) => {
        row.style.opacity = '0';
        row.style.transform = 'translateY(10px)';
        setTimeout(() => {
            row.style.opacity = '1';
            row.style.transform = 'translateY(0)';
        }, (index + sections.length) * 100);
    });
}); 