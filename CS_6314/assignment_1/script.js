/* 
 * Filename: script.js
 * Date: 09/09/23
 * Author: Sanjeev Penupala
 * Email: sanjeev.penupala@utdallas.edu
 * Course: CS 6314.002 Fall 2023
 * Version: 1.0
 * Copyright: 2023, All Rights Reserved  
 */

document.addEventListener("DOMContentLoaded", function () {
    const generateButton = document.getElementById("generate-btn");
    const resetButton = document.getElementById("reset-btn");
    const memeTextTopCard = document.getElementById("top-meme-card");
    const memeTextBottomCard = document.getElementById("bottom-meme-card");
    const memeTextTopInput = document.getElementById("meme-text-top-input");
    const memeTextBottomInput = document.getElementById("meme-text-bottom-input");

    generateButton.addEventListener("click", function () {
        const topText = memeTextTopInput.value;
        const bottomText = memeTextBottomInput.value;

        const memeTextTopElement = document.getElementById("meme-text-top");
        const memeTextBottomElement = document.getElementById("meme-text-bottom");

        const memeTextTopDiv = memeTextTopCard.querySelector(".card-body");
        const memeTextBottomDiv = memeTextBottomCard.querySelector(".card-body");

        if (topText) {
            memeTextTopElement.textContent = topText;
            memeTextTopDiv.style.display = "block";
        }
        if (bottomText) {
            memeTextBottomElement.textContent = bottomText;
            memeTextBottomDiv.style.display = "block";
        }

        generateButton.classList.add("active");
        resetButton.classList.remove("active");
    });

    resetButton.addEventListener("click", function () {
        memeTextTopInput.value = "";
        memeTextBottomInput.value = "";

        const memeTextTopDiv = memeTextTopCard.querySelector(".card-body");
        const memeTextBottomDiv = memeTextBottomCard.querySelector(".card-body");
        
        memeTextTopDiv.style.display = "none";
        memeTextBottomDiv.style.display = "none";

        memeTextTopInput.focus();
        memeTextTopInput.select();

        generateButton.classList.remove("active");
        resetButton.classList.add("active");
    });
});
