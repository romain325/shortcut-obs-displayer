const keyboard_div = document.getElementById("keyboard_shortcut");
const keyboard_text = document.getElementById("keyboard_title");
let last_shortcut = "";
let timeout = -1;

setInterval(async() => {
    const txt = await (await fetch("/shortcut", { cache: "no-store" })).text();
    console.log(txt);
    if(keyboard_text != null && keyboard_div != null && last_shortcut != txt) {
        if(timeout != -1) {
            clearTimeout(timeout);
            timeout = -1;
        }
        last_shortcut = txt;
        keyboard_text.innerText = txt;
        keyboard_div.style.visibility = "visible";
        timeout = setTimeout(() => {
            timeout = -1;

            keyboard_text.innerText = "";
            keyboard_div.style.visibility = "hidden";
        }, 3000)
    }
}, 200);
