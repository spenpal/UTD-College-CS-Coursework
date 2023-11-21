import bcrypt from "bcrypt";
import express from "express";
import passport from "passport";

import db from "../config/db.js";

const router = express.Router();

router.post("/signin", (req, res, next) => {
    passport.authenticate("local", (err, user, info) => {
        if (err) return next(err);
        if (!user) return res.status(400).send(info.message);

        req.logIn(user, (err) => {
            if (err) return next(err);
            return res.status(200).redirect("/home");
        });
    })(req, res, next);
});

router.post("/signup", async (req, res) => {
    const { username, email, phone, address, password } = req.body;

    // Check if the user already exists
    const existingUser = await db.collection("users").findOne({ username });
    if (existingUser) {
        return res
            .status(400)
            .send("Registration failed! Username is already taken.");
    }

    // Hash and salt the password
    const saltRounds = 10;
    const hashedPassword = await bcrypt.hash(password, saltRounds);

    // Create the user
    await db.collection("users").insertOne({
        username,
        email,
        phone,
        address,
        password: hashedPassword,
    });

    // Send the response
    res.status(201).send("Registration successful.");
});

export default router;
