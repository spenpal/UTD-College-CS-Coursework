/**
 * -------------- IMPORTS / CONFIGS ----------------
 */
import './config/env.js';
import './config/passport.js';

import MongoStore from 'connect-mongo';
import express from 'express';
import session from 'express-session';
import passport from 'passport';

import { client } from './config/db.js';
import authRoutes from './routes/auth.js';
import clientRoutes from './routes/client.js';

const app = express();
app.set("view engine", "ejs");

/**
 * -------------- MIDDLEWARE ----------------
 */
app.use(express.json());
app.use(express.urlencoded({ extended: true }));
app.use(
    session({
        secret: process.env.SESSION_SECRET,
        resave: false,
        saveUninitialized: true,
        store: MongoStore.create({ client: client }),
        cookie: {
            maxAge: 1000 * 60 * 10, // 10 minutes
        },
    })
);
app.use(passport.initialize());
app.use(passport.session());

/**
 * -------------- SERVER ROUTES ----------------
 */
app.use("/api/auth", authRoutes);

/**
 * -------------- CLIENT ROUTES ----------------
 */
app.use("/", clientRoutes);

/**
 * -------------- GLOBAL ERROR HANDLING ----------------
 */
app.use(function (err, req, res, next) {
    console.log(err);
});

/**
 * -------------- SERVER ----------------
 */
app.listen(process.env.PORT, () => {
    console.log(`Server started on port ${process.env.PORT || 3000}`);
});
