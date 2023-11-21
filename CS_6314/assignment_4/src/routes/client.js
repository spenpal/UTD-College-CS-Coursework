import express from "express";

const router = express.Router();

const isAuth = (req, res, next) => {
    if (req.isAuthenticated()) {
        res.redirect("/home");
    } else {
        next();
    }
};

router.get("/", isAuth, (req, res) => {
    res.redirect("/login");
});

router.get("/login", isAuth, (req, res) => {
    res.render("login");
});

router.get("/register", isAuth, (req, res) => {
    res.render("register");
});

router.get("/home", (req, res) => {
    if (req.isAuthenticated()) {
        res.render("home", { user: req.user });
    } else {
        res.status(401).send(
            "<h1>Unauthorized 401: You are not logged in.<h1>"
        );
    }
});

router.post("/logout", (req, res) => {
    if (req.isAuthenticated()) {
        req.logout((err) => {
            if (err) return next(err);
            res.status(200).redirect("/login");
        });
    } else {
        res.status(401).send(
            "<h1>Unauthorized 401: You are not logged in.<h1>"
        );
    }
});

export default router;
