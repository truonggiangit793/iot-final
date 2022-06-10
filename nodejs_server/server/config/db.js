const mongoose = require("mongoose");
const uri = process.env.DB_URL;

/* Connect to cluster mongoDB */
const connect = async function () {
  try {
    const options = { useNewUrlParser: true, useUnifiedTopology: true };
    await mongoose.connect(uri, options, () => {
      console.log("✅ Connect to mongoDB successfully!\n\n");
    });
  } catch (error) {
    console.log("❌ Failed to connect to MongoDB!");
    console.log(error.message);
  }
};

module.exports = { connect };
