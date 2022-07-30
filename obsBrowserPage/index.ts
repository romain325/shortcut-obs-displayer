const keyboard_div = document.getElementById("keyboard_shortcut");
const keyboard_text = document.getElementById("keyboard_title");
let last_shortcut = "";
let timeout = -1;

const remove_prefix = (str :  string) : string => {
    return str.replace(/KEY_/gi, "");
};

const to_azerty_map = (str: string): string => {
    switch (str) {
        case "Q":
            return "A";
        case "W":
            return "Z";
        case "A":
            return "Q";
        case "Z":
            return "W";
        case "SEMICOLON":
            return "M";
        case "APOSTROPHE":
            return "%";
        case "M":
            return ",";
        case "COMMA":
            return ";";
        case "DOT":
            return ":";
        case "SLASH":
            return "!";
        case "LEFTBRACE":
            return "^";
        case "RIGHTBRACE":
            return "$";
        case "BACKSLASH":
            return "*";
        default:
            return str;
    }
};


setInterval(async() => {
    const txt = await (await fetch("/shortcut", { cache: "no-store" })).text();
    console.log(txt);
    if(keyboard_text != null && keyboard_div != null && last_shortcut != txt) {
        if(timeout != -1) {
            clearTimeout(timeout);
            timeout = -1;
        }
        last_shortcut = txt;
        keyboard_text.innerText = remove_prefix(txt).split(" + ").map(to_azerty_map).join(" + ");
        keyboard_div.style.visibility = "visible";
        timeout = setTimeout(() => {
            timeout = -1;

            keyboard_text.innerText = "";
            keyboard_div.style.visibility = "hidden";
        }, 3000)
    }
}, 200);
