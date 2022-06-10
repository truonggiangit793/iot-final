const mongoose = require("mongoose");

const accessSchema = new mongoose.Schema(
  {
    status: { type: String },
    uid: { type: String },
  },
  {
    timestamps: true,
  }
);

module.exports = mongoose.model("Access", accessSchema);
