// Transform JSON to have the following key-value pair structure - {name: img_url}
function transformJSON(jsonData) {
    const games = jsonData.games;
    const transformedJSON = {};

    for (let game of games) {
        transformedJSON[game.name] = game.imagePath;
    }

    return transformedJSON;
}

// Function to populate the table from games.xml
async function populateTable() {
    const xmlText = await (await fetch("./data/games.xml")).text();
    const jsonData = await (await fetch("./data/games.json")).json();
    const gameImages = transformJSON(jsonData);

    const parser = new DOMParser();
    const xmlDoc = parser.parseFromString(xmlText, "text/xml");
    const games = xmlDoc.getElementsByTagName("game");
    const tableBody = $("#gameTable tbody");

    $.each(games, function (i, game) {
        const name = game.getElementsByTagName("name")[0].textContent;
        const description =
            game.getElementsByTagName("description")[0].textContent;

        const pricing = game.getElementsByTagName("pricing")[0];
        const hourly = pricing.getElementsByTagName("hourly")[0].textContent;
        const perGame = pricing.getElementsByTagName("perGame")[0].textContent;

        const image = game.getElementsByTagName("image")[0];
        const image_description =
            image.getElementsByTagName("description")[0].textContent;

        const rowHTML = `<tr>
        <td class="align-middle">${name}</td>
        <td class="align-middle">${description}</td>
        <td class="align-middle">${hourly}</td>
        <td class="align-middle">${perGame}</td>
        <td><img src="${gameImages[name]}" alt="${image_description}" class="img-fluid"></td>
        </tr>`;
        tableBody.append(rowHTML);
    });
}

// Populate the table from games.xml and games.json on page load
$(document).ready(function () {
    populateTable();

    // Function to toggle image visibility
    $("#toggleImagesBtn").click(function () {
        $("#gameTable thead tr th:last-child").toggle();
        $("#gameTable tbody tr td:last-child").toggle();
        $(this).text(function (i, text) {
            return text === "Show Images" ? "Hide Images" : "Show Images";
        });
    });
});
