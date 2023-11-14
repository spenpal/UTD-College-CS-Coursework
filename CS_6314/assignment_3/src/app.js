// IMPORTS
const express = require("express");
const { MongoClient, ObjectId } = require("mongodb");
const app = express();
const PORT = 3000;

// Express Middleware
app.use(express.json());
app.use(express.urlencoded({ extended: true }));
app.set("view engine", "ejs");
app.use(express.static("public"));

// MongoDB Connection
const uri = "mongodb://localhost:27017"; // Connection string
const dbName = "arcade"; // Database name
const collectionName = "games"; // Collection name
const client = new MongoClient(uri);
async function connect() {
    try {
        await client.connect();
    } catch (e) {
        console.error(e);
    }
}
connect();
const gamesCollection = client.db(dbName).collection(collectionName);

// ROUTES
// Base Route
app.get("/", (req, res) => {
    res.redirect("/games");
});

// List all games
app.get("/games", async (req, res) => {
    // Retrieve all games from the database and sort them by name
    try {
        const games = await gamesCollection.find().sort({ name: 1 }).toArray();
        const uniqueTypes = await gamesCollection.distinct("type");
        res.render("index", { games, typeList: uniqueTypes });
    } catch (err) {
        console.error(err);
        res.status(500).send("Internal Server Error");
    }
});

// Search games
app.get("/games/search", async (req, res) => {
    try {
        const { name, type } = req.query;
        let query = {};
        if (name) {
            query.name = { $regex: new RegExp(name, "i") };
        }
        if (type && type !== "No filter") {
            query.type = type;
        }
        const games = await gamesCollection
            .find(query)
            .sort({ name: 1 })
            .toArray();
        const uniqueTypes = await gamesCollection.distinct("type");
        res.render("index", { games, typeList: uniqueTypes });
    } catch (err) {
        console.error(err);
        res.status(500).send("Internal Server Error");
    }
});

// Show new game form
app.get("/games/new", (req, res) => {
    res.render("new");
});

// Create new game
app.post("/games", async (req, res) => {
    try {
        await gamesCollection.insertOne(req.body);
        res.status(200).send("Game created successfully");
    } catch (err) {
        console.error(err);
        res.status(500).send("Internal Server Error");
    }
});

// Show information about specific game
app.get("/games/:id", async (req, res) => {
    try {
        const game = await gamesCollection
            .find({ _id: new ObjectId(req.params.id) })
            .toArray();
        res.render("show", { game: game[0] });
    } catch (err) {
        console.error(err);
        res.status(500).send("Internal Server Error");
    }
});

// Edit specific game
app.get("/games/:id/edit", async (req, res) => {
    try {
        const game = await gamesCollection
            .find({ _id: new ObjectId(req.params.id) })
            .toArray();
        res.render("edit", { game: game[0] });
    } catch (err) {
        console.error(err);
        res.status(500).send("Internal Server Error");
    }
});

// Update specific game
app.put("/games/:id", async (req, res) => {
    try {
        await gamesCollection.updateOne(
            { _id: new ObjectId(req.params.id) },
            { $set: req.body }
        );
        res.status(200).send("Game updated successfully");
    } catch (err) {
        console.error(err);
        res.status(500).send("Internal Server Error");
    }
});

// Delete specific game
app.delete("/games/:id", async (req, res) => {
    try {
        await gamesCollection.deleteOne({
            _id: new ObjectId(req.params.id),
        });
        res.status(200).send("Game deleted successfully");
    } catch (err) {
        console.error(err);
        res.status(500).send("Internal Server Error");
    }
});

// Run server
app.listen(PORT, (err) => {
    if (err) console.log(err);
    console.log("Server listening on PORT", PORT);
});

// Handle process exit or Ctrl+C
process.on("SIGINT", () => {
    // Close MongoDB connection before exiting
    client.close().then(() => {
        console.log("MongoDB connection closed");
        process.exit(0);
    });
});
