To run this project, make sure you have MongoDB server installed on your system and you have started it up at the default address: `mongodb://localhost:27017/<db_name>`.

-   NOTE: The names were used for `db` and `collections` and were created through the `MongoDB VSCode` extension.
    -   **db_name**: `auth`
    -   **collections**: [`users`, `sessions`]

Make sure to follow the instructions, to create a .env file, at `./config/config.env`.

Then, go into the `src` folder and run `npm install`, followed by `npm run dev`. The project will open up at [http://localhost:3000/](http://localhost:3000/).
