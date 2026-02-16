function changeState(mainId, id1, id2) {
    const mainIdClassList = document.querySelector(`#${mainId}`).classList;
    const id1ClassList = document.querySelector(`#${id1}`).classList;
    const id2ClassList = document.querySelector(`#${id2}`).classList;

    const allArentToggled = !mainIdClassList.contains('is-toggled') && !id1ClassList.contains('is-toggled') && !id2ClassList.contains('is-toggled');
    if (allArentToggled) {
        mainIdClassList.add('is-toggled');
    } else {
        mainIdClassList.remove('is-toggled');
    }
}   