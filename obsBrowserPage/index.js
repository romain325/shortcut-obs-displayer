"use strict";
var __awaiter = (this && this.__awaiter) || function (thisArg, _arguments, P, generator) {
    function adopt(value) { return value instanceof P ? value : new P(function (resolve) { resolve(value); }); }
    return new (P || (P = Promise))(function (resolve, reject) {
        function fulfilled(value) { try { step(generator.next(value)); } catch (e) { reject(e); } }
        function rejected(value) { try { step(generator["throw"](value)); } catch (e) { reject(e); } }
        function step(result) { result.done ? resolve(result.value) : adopt(result.value).then(fulfilled, rejected); }
        step((generator = generator.apply(thisArg, _arguments || [])).next());
    });
};
const keyboard_div = document.getElementById("keyboard_shortcut");
const keyboard_text = document.getElementById("keyboard_title");
let last_shortcut = "";
let timeout = -1;
setInterval(() => __awaiter(void 0, void 0, void 0, function* () {
    const txt = yield (yield fetch("/shortcut", { cache: "no-store" })).text();
    console.log(txt);
    if (keyboard_text != null && keyboard_div != null && last_shortcut != txt) {
        if (timeout != -1) {
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
        }, 3000);
    }
}), 200);
