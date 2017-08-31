var assert = require('assert'),
  chai = require('chai'),
  expect = chai.expect,
  should = chai.should(),
  analytics = require('../../index'),
  candlesticks = require('../candlesticks');
  strategy = require("../strategy");


describe('analytics', function() {
  describe('#getMarketStatus', function() {
    var status;
    before(function(done) {
        status = analytics.getMarketStatus(
            {
                "IND1": candlesticks.splice(0, 60).map(c => c.open),
                "IND2": candlesticks.splice(0, 60).map(c => -c.high)
            },
            {strategy}
        );
        //console.log(status);
        done()
    });
    it('return value exists', function() {expect(status).to.not.be.undefined});
    it('return value .shouldBuy exists', function() {expect(status.shouldBuy).to.not.be.undefined});
    it('return value .shouldSell exists', function() {expect(status.shouldSell).to.not.be.undefined});
  });
  describe('#getMarketStatus validations', function() {
      it('unknown indicator', function(done) {
        try {
          analytics.getMarketStatus(
              {
                  "IND1": candlesticks.splice(0, 60).map(c => c.open),
                  "IND3": candlesticks.splice(0, 60).map(c => -c.high)
              },
              {strategy}
          );
          done(new Error("accepted indicator data missing a strategy indicator"))
        } catch (e) {
          assert.equal(e.message, "could not find strategy indicator 'IND2' in indicator data");
          done();
        }
      });
  });
});
