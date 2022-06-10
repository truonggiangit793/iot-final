/* Routes for apiRoute */
const apiRoute = require("express").Router();
const accessModel = require("../models/Access.model");

// Root: http://example.com/api/v1

apiRoute.get("/new-data", async (req, res, next) => {
  const uid = req.query.uid || null;
  const accessStatus = req.query.status || null;
  console.log({uid});
  console.log({accessStatus});
  if (accessStatus === "0" || accessStatus === "1") {
    new accessModel({ status: accessStatus == "1" ? "Success" : "Dennied", uid })
      .save()
      .then((inserted) => {
        console.log(inserted);
        if (inserted) return res.json({ status: true, message: "Success" });
        return res.json({ status: false, message: "Failed" });
      })
      .catch((err) => {
        return res.json({ status: false, message: "Failed" });
      });
  } else {
    return res.json({ status: false, message: "Invalid status!" });
  }
});

apiRoute.get("/get-data", (req, res, next) => {
  accessModel
    .find({})
    .then((data) => {
      return res.json({ status: true, message: "Success", data });
    })
    .catch((err) => {
      return res.json({ status: false, message: "Failed" });
    });
});

module.exports = apiRoute;
