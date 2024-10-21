document.addEventListener('DOMContentLoaded', function() {
    const themeLink = document.getElementById('theme-link');
    const megahackLink = document.getElementById('megahack-link');
    const megaOverlayLink = document.getElementById('gdmegaoverlay-link');
    
    const savedTheme = localStorage.getItem('selected-theme');
    if (savedTheme) {
        themeLink.href = savedTheme;
    }

    megahackLink.addEventListener('click', (e) => {
        e.preventDefault();
        themeLink.href = 'assets/css/megahack.css';
        localStorage.setItem('selected-theme', 'assets/css/megahack.css');
    });

    megaOverlayLink.addEventListener('click', (e) => {
        e.preventDefault();
        themeLink.href = 'assets/css/megaoverlay.css';
        localStorage.setItem('selected-theme', 'assets/css/megaoverlay.css');
    });
});