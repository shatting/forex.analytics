var assert = require('assert'),
  chai = require('chai'),
  expect = chai.expect,
  should = chai.should(),
  analytics = require('../../index'),
  candlesticks = require('../candlesticks');


var indicators = [
  'CCI',
  'MACD',
  'MACD_Signal',
  'MACD_Histogram',
  'Momentum',
  'RSI',
  'BOP',
  'ATR',
  'SAR',
  'SMA15_SMA50',
  'Stochastic'
];

describe('analytics', function() {
  describe('#findStrategy', function() {

    describe.skip('()', function() {
      var error;

      before(function(done) {
        analytics.findStrategy()
          .then(function() {
            error = null;
            done();
          })
          .catch(function(e) {
            error = e;
            done();
          });
      });

      it('should throw validation error because close value parameter is required', function() {
        expect('' + error).to.be.equal('TypeError: Wrong first argument. Expecting array of close values');
      });
    });

    describe.skip('(candlesticks)', function() {
      var error;
      var strategy;

      before(function(done) {

        this.timeout(0);

        analytics.findStrategy(candlesticks.splice(0, 60))

        .then(function(s) {
          strategy = s;
          error = null;
          done();
        })

        .catch(function(e) {
          strategy = null;
          error = e;
          done();
        });
      });

      it('should throw validation error because options parameter is required', function() {
        expect('' + error).to.be.equal('TypeError: Wrong second argument. Expecting object with genetic algorithm configuration');
      });
    });

    describe('(closeValues)', function() {
      var error;
      var strategy;

      before(function(done) {

        this.timeout(0);

        analytics.findStrategy(candlesticks.splice(0, 60).map(c => c.close))

        .then(function(s) {
          strategy = s;
          error = null;
          done();
        })

        .catch(function(e) {
          strategy = null;
          error = e;
          done();
        });
      });

      it('should throw validation error because options parameter is required', function() {
        expect('' + error).to.be.equal('TypeError: Wrong second argument. Expecting object {\'INDICATOR1\':[x1,y1,..], \'INDICATOR2\':[x2,y2,..]}');
      });
    });

    describe.skip('(candlesticks, { })', function() {
      var error;
      var strategy;

      before(function(done) {

        this.timeout(0);

        analytics.findStrategy(candlesticks.splice(0, 60))

        .then(function(s) {
          strategy = s;
          error = null;
          done();
        })

        .catch(function(e) {
          strategy = null;
          error = e;
          done();
        });
      });

      it('should throw validation error because options parameter is required', function() {
        expect('' + error).to.be.equal('TypeError: Wrong second argument. Expecting object with genetic algorithm configuration');
      });
    });

    describe.skip('(candlesticks, { pipInDecimals : 0 })', function() {
      var error;
      var strategy;

      before(function(done) {

        this.timeout(0);

        analytics.findStrategy(candlesticks, { pipInDecimals: 0 })

        .then(function(s) {
          strategy = s;
          error = null;
          done();
        })

        .catch(function(e) {
          strategy = null;
          error = e;
          done();
        });
      });

      it('should throw validation error because pipInDecimals can\'t be zero or lower', function() {
        expect('' + error).to.be.equal('TypeError: Expecting pipInDecimals to be larger than 0');
      });
    });

    describe.skip('(candlesticks, { pipInDecimals : 0.0001 })', function() {
      var error;
      var strategy;

      before(function(done) {

        this.timeout(0);

        analytics.findStrategy(candlesticks, { pipInDecimals : 0.001 })

        .then(function(s) {
          strategy = s;
          error = null;
          done();
        })

        .catch(function(e) {
          strategy = null;
          error = e;
          done();
        });
      });

      it('should not throw validation error', function() {
        expect(error).to.be.equal(null);
      });

      it('should return strategy object', function() {
        expect(strategy).to.not.be.equal(null);
      });

      it('should return strategy object with buy strategy', function() {
        expect(strategy.buy).to.not.be.equal(null);
      });

      it('should return strategy object with sell strategy', function() {
        expect(strategy.sell).to.not.be.equal(null);
      });
    });

    describe('(closeValues, indicators, options, callback)', function() {
      var error;
      var strategy;
      var statusCounter = 0;
      var generationCount = 50;
      var pipInDecimals = 0.001;
      var spread = 0.2
      var options = {generationCount, pipInDecimals, spread};

      before(function(done) {

        this.timeout(0);

        analytics.findStrategy(candlesticks.splice(0, 60).map(c => c.close), {
            "IND1": candlesticks.splice(0, 60).map(c => c.open),
            "IND2": candlesticks.splice(0, 60).map(c => -c.high)
        }, options ,(strategy, fitness, generation) => {
          statusCounter++;
          //console.log(statusCounter, fitness, strategy, generation);
        }).then(function(s) {
          strategy = s;
          //console.log(JSON.stringify(s, false, 4));
          error = null;
          done();
        })

        .catch(function(e) {
          strategy = null;
          error = e;
          done();
        });
      });

      it('should not throw validation error', function() {
        expect(error).to.be.equal(null);
      });

      it('should return strategy object', function() {
        expect(strategy).to.not.be.equal(null);
      });

      it('should return strategy object with buy strategy', function() {
        expect(strategy.buy).to.not.be.equal(null);
      });

      it('should return strategy object with sell strategy', function() {
        expect(strategy.sell).to.not.be.equal(null);
      });

      // https://github.com/nodejs/nan/issues/642
/*      it('should fire status callback with each completed generation', function() {
        expect(statusCounter).to.be.equal(generationCount);
      });*/
    });

    /*for (var indicator of indicators) {
      describe('(candlesticks, options)', function() {

        var error = null;

        before(function(done) {

          this.timeout(0);

          analytics.findStrategy([], {
            indicators: [indicator],
            pipInDecimals : 0.001
          })

          .then(function(s) {
            error = null;
            done();
          })

          .catch(function(e) {
            error = e;
            done();
          });
        });

        it('should throw an error when ' + indicator + ' indicator doesn\'t get sufficient number of candlesticks to perform', function() {
          expect('' + error).to.be.equal('TypeError: Unsufficient amount of input candlesticks');
        });
      });
    }*/
  });
});
