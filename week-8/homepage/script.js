// wait for the dom to be fully loaded
document.addEventListener('DOMContentLoaded', function() {
    // initialize tooltips
    $('[data-toggle="tooltip"]').tooltip();

    // load footer
    fetch('footer.html')
        .then(response => response.text())
        .then(data => {
            document.body.insertAdjacentHTML('beforeend', data);
            // set current year in copyright
            const currentYear = new Date().getFullYear();
            document.getElementById('copyright-year').textContent = currentYear;
        })
        .catch(error => console.error('error loading footer:', error));

    // initialize contact information if on contact page
    if (window.location.pathname.includes('contact.html')) {
        // update email
        const emailLink = document.getElementById('email-link');
        if (emailLink) {
            emailLink.href = `mailto:${USER_INFO.email}`;
            emailLink.textContent = USER_INFO.email;
        }

        // update location
        const locationText = document.getElementById('location-text');
        if (locationText) {
            locationText.textContent = USER_INFO.location;
        }

        // update availability
        const availabilityText = document.getElementById('availability-text');
        if (availabilityText) {
            availabilityText.textContent = USER_INFO.availability;
        }
    }

    // smooth scrolling for navigation links
    document.querySelectorAll('a[href^="#"]').forEach(anchor => {
        anchor.addEventListener('click', function(e) {
            e.preventDefault();
            const target = document.querySelector(this.getAttribute('href'));
            if (target) {
                target.scrollIntoView({
                    behavior: 'smooth',
                    block: 'start'
                });
            }
        });
    });

    // form validation and submission
    const contactForm = document.getElementById('contactForm');
    if (contactForm) {
        contactForm.addEventListener('submit', function(e) {
            e.preventDefault();
            
            // get form values
            const name = document.getElementById('name').value;
            const email = document.getElementById('email').value;
            const subject = document.getElementById('subject').value;
            const message = document.getElementById('message').value;

            // simple validation
            if (!name || !email || !subject || !message) {
                alert('please fill in all fields');
                return;
            }

            // email validation
            const emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
            if (!emailRegex.test(email)) {
                alert('please enter a valid email address');
                return;
            }

            // simulate form submission
            const submitButton = contactForm.querySelector('button[type="submit"]');
            const originalText = submitButton.textContent;
            
            submitButton.disabled = true;
            submitButton.textContent = 'sending...';

            // simulate api call
            setTimeout(() => {
                submitButton.textContent = 'message sent!';
                contactForm.reset();
                
                setTimeout(() => {
                    submitButton.disabled = false;
                    submitButton.textContent = originalText;
                }, 3000);
            }, 1500);
        });
    }

    // add animation class to elements when they come into view
    const animateOnScroll = function() {
        const elements = document.querySelectorAll('.card, .skill-card, .contact-info .info-item');
        
        elements.forEach(element => {
            const elementPosition = element.getBoundingClientRect().top;
            const windowHeight = window.innerHeight;
            
            if (elementPosition < windowHeight - 100) {
                element.classList.add('animate');
            }
        });
    };

    // run animation check on scroll
    window.addEventListener('scroll', animateOnScroll);
    // run once on page load
    animateOnScroll();

    // add active class to current navigation item
    const currentLocation = location.pathname;
    const navLinks = document.querySelectorAll('.nav-link');
    
    navLinks.forEach(link => {
        if (link.getAttribute('href') === currentLocation) {
            link.classList.add('active');
        } else {
            link.classList.remove('active');
        }
    });

    // add hover effect to project cards
    const projectCards = document.querySelectorAll('.projects-grid .card');
    projectCards.forEach(card => {
        card.addEventListener('mouseenter', function() {
            this.style.transform = 'translateY(-10px)';
            this.style.boxShadow = '0 15px 30px rgba(0, 0, 0, 0.1)';
        });

        card.addEventListener('mouseleave', function() {
            this.style.transform = 'translateY(0)';
            this.style.boxShadow = '0 4px 6px rgba(0, 0, 0, 0.1)';
        });
    });

    // add loading animation to images
    const images = document.querySelectorAll('img');
    images.forEach(img => {
        img.addEventListener('load', function() {
            this.style.opacity = '1';
        });
        
        if (img.complete) {
            img.style.opacity = '1';
        } else {
            img.style.opacity = '0';
            img.style.transition = 'opacity 0.3s ease';
        }
    });
}); 