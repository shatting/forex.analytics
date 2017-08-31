module.exports = {
    "buy": {
    "operator": "And",
        "left": {
        "indicator": "IND2",
            "sign": ">",
            "value": -0.026547946573777747
    },
    "right": {
        "indicator": "IND1",
            "sign": "<",
            "value": 1.0642046755770533
    }
},
    "sell": {
    "operator": "Or",
        "left": {
        "indicator": "IND1",
            "sign": "<",
            "value": 1.1207923838782399
    },
    "right": {
        "indicator": "IND1",
            "sign": ">",
            "value": 1.0928699412795786
    }
}
};