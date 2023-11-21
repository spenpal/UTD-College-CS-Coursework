import { MongoClient } from "mongodb";

const connectionString = process.env.MONGODB_URI || "";

const client = new MongoClient(connectionString);

try {
    await client.connect();
} catch (e) {
    console.error(e);
}

let db = client.db("auth");

export default db;
export { client };
