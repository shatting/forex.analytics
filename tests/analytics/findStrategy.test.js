var assert = require('assert'),
  chai = require('chai'),
  expect = chai.expect,
  should = chai.should(),
  analytics = require('../../index'),
  candlesticks = require('../candlesticks');

require("segfault-handler").registerHandler("crash.log");

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

    describe('()', function() {
      it('should throw validation error because close value parameter is required', function() {
          assert.throws(function(){
              analytics.findStrategy()
          }, /Wrong number of arguments/);
      });
    });

    describe('(closeValues)', function() {
      it('should throw validation error because indicatorValues parameter is required', function() {
          assert.throws(function(){
              analytics.findStrategy(candlesticks.splice(0, 60).map(c => c.close))
          }, /Wrong number of arguments/);
      });
    });

      describe('(closeValues,indicatorValues)', function() {
          it('should throw validation error because options parameter is required', function() {
              assert.throws(function(){
                  analytics.findStrategy(candlesticks.splice(0, 60).map(c => c.close),{"ind1":candlesticks.splice(0, 60).map(c => c.close)})
              }, /Wrong number of arguments/);
          });
      });
      describe('(closeValues,indicatorValues,options)', function() {
          it('should throw validation error because updateCb parameter is required', function() {
              assert.throws(function(){
                  analytics.findStrategy(candlesticks.splice(0, 60).map(c => c.close),{"ind1":candlesticks.splice(0, 60).map(c => c.close)},{})
              }, /Wrong number of arguments/);
          });
      });
      describe('(closeValues,indicatorValues,options,updateCb)', function() {
          it('should throw validation error because cb parameter is required', function() {
              assert.throws(function(){
                  analytics.findStrategy(candlesticks.splice(0, 60).map(c => c.close),{"ind1":candlesticks.splice(0, 60).map(c => c.close)},{},function(){});
              }, /Wrong number of arguments/);
          });
      });

    describe('(closeValues, indicatorValues, options, updateCb, cb)', function() {
      var error;
      var strategy;
      var statusCounter = 0;
      var generationCount = 5;
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
          console.log(statusCounter, fitness, strategy, generation);
        },s => {
          strategy = s;
          console.log(JSON.stringify(s, false, 4));
          error = null;
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
